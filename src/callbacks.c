/*
 * Ownership, the stream callback queue, logging, assertions, fork.
 *
 * sokol-audio is one process-wide instance. The PHP thread that calls
 * saudio_setup() owns it until saudio_shutdown() or request end.
 *
 * sokol-audio calls the stream callback on its own audio thread, which never
 * enters PHP. The trampoline posts the request (buffer, frame count, channel
 * count), raises the owner's EG(vm_interrupt) and waits. The owner serves the
 * request from zend_interrupt_function at its next VM interrupt point: it
 * calls the PHP closure with the buffer as a string of packed floats and
 * copies the string back. A request the owner has not started serving within
 * one buffer's duration (num_frames / sample rate) is withdrawn and the
 * buffer is filled with silence. Once started, the audio thread waits for the
 * closure to return.
 *
 * Log calls on the owner thread (all of setup) call the PHP logger directly.
 * Log calls on the audio thread are queued and delivered at the owner's next
 * VM interrupt point, or by saudio_shutdown().
 */

#include "binding.h"

#include "zend_exceptions.h"

#include <errno.h>
#include <math.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

zend_class_entry *phpsaudio_ce_assertion_error;
phpsaudio_held phpsaudio_values;

enum { REQ_IDLE = 0, REQ_POSTED, REQ_STARTED };

typedef struct phpsaudio_log {
	/* sokol-audio passes string literals and its static message table: stored as pointers. */
	const char *tag;
	uint32_t log_level;
	uint32_t log_item_id;
	const char *message;
	uint32_t line_nr;
	const char *filename;
	struct phpsaudio_log *next;
} phpsaudio_log;

static struct {
	pthread_mutex_t lock;
	pthread_cond_t cond;

	bool setup;      /* saudio_setup() was called, saudio_shutdown() not yet */
	bool dead;       /* forked child of a process with a live audio thread */
	bool stopping;   /* saudio_shutdown() in progress: no new stream requests */
	pthread_t owner;
	zend_atomic_bool *owner_interrupt;
	int sample_rate;

	int req_state;
	bool req_userdata;
	float *req_buffer;
	int req_frames;
	int req_channels;

	phpsaudio_log *logs_head;
	phpsaudio_log *logs_tail;

	bool assert_pending;
	const char *assert_condition;
	const char *assert_file;
	int assert_line;
} st = {
	.lock = PTHREAD_MUTEX_INITIALIZER,
	.cond = PTHREAD_COND_INITIALIZER,
};

/* Owner thread only: PHP callbacks running now. saudio_setup()/saudio_shutdown() inside one would deadlock the audio thread. */
static int callback_depth;

static void (*prev_interrupt_function)(zend_execute_data *execute_data);

static bool is_owner_locked(void)
{
	return st.setup && pthread_equal(st.owner, pthread_self());
}

/* ---- assertions ---- */

void phpsaudio_on_assert(const char *condition, const char *file, int line)
{
	pthread_mutex_lock(&st.lock);
	if (!st.assert_pending) {
		const char *slash = strrchr(file, '/');
		st.assert_pending = true;
		st.assert_condition = condition;
		st.assert_file = slash ? slash + 1 : file;
		st.assert_line = line;
	}
	pthread_mutex_unlock(&st.lock);
}

void phpsaudio_assert_check(void)
{
	pthread_mutex_lock(&st.lock);
	bool pending = st.assert_pending;
	const char *condition = st.assert_condition, *file = st.assert_file;
	int line = st.assert_line;
	st.assert_pending = false;
	pthread_mutex_unlock(&st.lock);

	if (pending && !EG(exception)) {
		zend_throw_exception_ex(phpsaudio_ce_assertion_error, 0, "%s at %s:%d", condition, file, line);
	}
}

/* ---- guards ---- */

static const char dead_message[] = "sokol-audio's audio thread did not survive fork";

zend_result phpsaudio_enter(void)
{
	pthread_mutex_lock(&st.lock);
	bool setup = st.setup, dead = st.dead, owner = is_owner_locked();
	pthread_mutex_unlock(&st.lock);

	if (dead) {
		zend_throw_error(NULL, "%s", dead_message);
		return FAILURE;
	}
	if (!setup) {
		zend_throw_error(NULL, "saudio_setup() has not been called");
		return FAILURE;
	}
	if (!owner) {
		zend_throw_error(NULL, "sokol-audio was set up by another thread");
		return FAILURE;
	}
	return SUCCESS;
}

zend_result phpsaudio_enter_setup(void)
{
	pthread_mutex_lock(&st.lock);
	if (st.dead) {
		pthread_mutex_unlock(&st.lock);
		zend_throw_error(NULL, "%s", dead_message);
		return FAILURE;
	}
	if (st.setup) {
		bool owner = is_owner_locked();
		pthread_mutex_unlock(&st.lock);
		if (owner) {
			zend_throw_error(NULL, "saudio_setup() was already called; call saudio_shutdown() first");
		} else {
			zend_throw_error(NULL, "sokol-audio was set up by another thread");
		}
		return FAILURE;
	}
	st.setup = true;
	st.owner = pthread_self();
	st.owner_interrupt = &EG(vm_interrupt);
	st.sample_rate = phpsaudio_default_sample_rate;
	st.req_state = REQ_IDLE;
	pthread_mutex_unlock(&st.lock);
	return SUCCESS;
}

void phpsaudio_release_setup(void)
{
	pthread_mutex_lock(&st.lock);
	st.setup = false;
	pthread_mutex_unlock(&st.lock);
}

void phpsaudio_setup_done(int sample_rate)
{
	pthread_mutex_lock(&st.lock);
	if (sample_rate > 0) {
		st.sample_rate = sample_rate;
	}
	pthread_mutex_unlock(&st.lock);
}

zend_result phpsaudio_enter_shutdown(void)
{
	if (phpsaudio_enter() == FAILURE) {
		return FAILURE;
	}
	if (callback_depth > 0) {
		zend_throw_error(NULL, "saudio_shutdown() cannot be called from inside a sokol-audio callback");
		return FAILURE;
	}
	return SUCCESS;
}

/* ---- calling PHP (owner thread) ---- */

static void release_held(void)
{
	zval_ptr_dtor(&phpsaudio_values.stream_cb);
	zval_ptr_dtor(&phpsaudio_values.stream_userdata_cb);
	zval_ptr_dtor(&phpsaudio_values.user_data);
	zval_ptr_dtor(&phpsaudio_values.logger_func);
	zval_ptr_dtor(&phpsaudio_values.logger_user_data);
	zval_ptr_dtor(&phpsaudio_values.allocator_user_data);
	ZVAL_NULL(&phpsaudio_values.stream_cb);
	ZVAL_NULL(&phpsaudio_values.stream_userdata_cb);
	ZVAL_NULL(&phpsaudio_values.user_data);
	ZVAL_NULL(&phpsaudio_values.logger_func);
	ZVAL_NULL(&phpsaudio_values.logger_user_data);
	ZVAL_NULL(&phpsaudio_values.allocator_user_data);
}

static void call_php(zval *fn, uint32_t argc, zval *argv)
{
	zval retval;

	ZVAL_UNDEF(&retval);
	callback_depth++;
	call_user_function(NULL, NULL, fn, &retval, argc, argv);
	callback_depth--;
	zval_ptr_dtor(&retval);
}

/* Fills buffer through the PHP stream closure. Silence when the closure throws or leaves a wrong $buffer. */
static void run_stream(bool userdata, float *buffer, int num_frames, int num_channels)
{
	size_t bytes = (size_t)num_frames * (size_t)num_channels * sizeof(float);
	zval *fn = userdata ? &phpsaudio_values.stream_userdata_cb : &phpsaudio_values.stream_cb;
	zval args[4], tmp;

	if (EG(exception) || Z_TYPE_P(fn) != IS_OBJECT) {
		memset(buffer, 0, bytes);
		return;
	}

	zend_string *s = zend_string_alloc(bytes, 0);
	memset(ZSTR_VAL(s), 0, bytes);
	ZSTR_VAL(s)[bytes] = '\0';
	ZVAL_STR(&tmp, s);
	ZVAL_NEW_REF(&args[0], &tmp);
	ZVAL_LONG(&args[1], num_frames);
	ZVAL_LONG(&args[2], num_channels);
	if (userdata) {
		ZVAL_COPY(&args[3], &phpsaudio_values.user_data);
	}

	call_php(fn, userdata ? 4 : 3, args);

	zval *out = Z_REFVAL(args[0]);
	if (EG(exception)) {
		memset(buffer, 0, bytes);
	} else if (Z_TYPE_P(out) != IS_STRING) {
		memset(buffer, 0, bytes);
		zend_type_error("sokol-audio stream callback must leave $buffer a string, %s given", zend_zval_value_name(out));
	} else if (Z_STRLEN_P(out) != bytes) {
		memset(buffer, 0, bytes);
		zend_value_error("sokol-audio stream callback must leave $buffer %zu bytes (%d frames x %d channels x 4), %zu given",
			bytes, num_frames, num_channels, Z_STRLEN_P(out));
	} else {
		memcpy(buffer, Z_STRVAL_P(out), bytes);
	}

	zval_ptr_dtor(&args[0]);
	if (userdata) {
		zval_ptr_dtor(&args[3]);
	}
}

static void run_logger(const char *tag, uint32_t log_level, uint32_t log_item_id, const char *message,
	uint32_t line_nr, const char *filename)
{
	zval args[7];

	if (EG(exception) || Z_TYPE(phpsaudio_values.logger_func) != IS_OBJECT) {
		return;
	}

	ZVAL_STRING(&args[0], tag ? tag : "");
	ZVAL_LONG(&args[1], log_level);
	ZVAL_LONG(&args[2], log_item_id);
	if (message) {
		ZVAL_STRING(&args[3], message);
	} else {
		ZVAL_NULL(&args[3]);
	}
	ZVAL_LONG(&args[4], line_nr);
	if (filename) {
		ZVAL_STRING(&args[5], filename);
	} else {
		ZVAL_NULL(&args[5]);
	}
	ZVAL_COPY(&args[6], &phpsaudio_values.logger_user_data);

	call_php(&phpsaudio_values.logger_func, 7, args);

	for (int i = 0; i < 7; i++) {
		zval_ptr_dtor(&args[i]);
	}
}

static phpsaudio_log *take_logs_locked(void)
{
	phpsaudio_log *head = st.logs_head;
	st.logs_head = st.logs_tail = NULL;
	return head;
}

static void deliver_logs(phpsaudio_log *log, bool call)
{
	while (log) {
		phpsaudio_log *next = log->next;
		if (call) {
			run_logger(log->tag, log->log_level, log->log_item_id, log->message, log->line_nr, log->filename);
		}
		free(log);
		log = next;
	}
}

/* Owner thread, at a VM interrupt point: serve the posted stream request, then queued logs. */
static void serve(void)
{
	pthread_mutex_lock(&st.lock);
	if (st.dead || !is_owner_locked()) {
		pthread_mutex_unlock(&st.lock);
		return;
	}
	if (st.req_state == REQ_POSTED && !EG(exception)) {
		bool userdata = st.req_userdata;
		float *buffer = st.req_buffer;
		int frames = st.req_frames, channels = st.req_channels;

		st.req_state = REQ_STARTED;
		pthread_mutex_unlock(&st.lock);

		run_stream(userdata, buffer, frames, channels);

		pthread_mutex_lock(&st.lock);
		st.req_state = REQ_IDLE;
		pthread_cond_broadcast(&st.cond);
	}
	phpsaudio_log *logs = take_logs_locked();
	pthread_mutex_unlock(&st.lock);

	deliver_logs(logs, true);
}

static void interrupt_function(zend_execute_data *execute_data)
{
	if (prev_interrupt_function) {
		prev_interrupt_function(execute_data);
	}
	serve();
}

/* ---- C callbacks handed to sokol-audio ---- */

static void stream_request(bool userdata, float *buffer, int num_frames, int num_channels)
{
	size_t bytes = (size_t)num_frames * (size_t)num_channels * sizeof(float);

	pthread_mutex_lock(&st.lock);
	if (!st.setup || st.stopping || st.dead) {
		pthread_mutex_unlock(&st.lock);
		memset(buffer, 0, bytes);
		return;
	}
	if (pthread_equal(st.owner, pthread_self())) {
		/* Backends without an audio thread call back from inside a sokol-audio call on the owner. */
		pthread_mutex_unlock(&st.lock);
		run_stream(userdata, buffer, num_frames, num_channels);
		return;
	}

	st.req_state = REQ_POSTED;
	st.req_userdata = userdata;
	st.req_buffer = buffer;
	st.req_frames = num_frames;
	st.req_channels = num_channels;
	zend_atomic_bool_store(st.owner_interrupt, true);

	double seconds = (double)num_frames / (double)st.sample_rate;
	struct timespec deadline;
	clock_gettime(CLOCK_REALTIME, &deadline);
	deadline.tv_sec += (time_t)seconds;
	deadline.tv_nsec += (long)((seconds - floor(seconds)) * 1e9);
	if (deadline.tv_nsec >= 1000000000L) {
		deadline.tv_sec += 1;
		deadline.tv_nsec -= 1000000000L;
	}

	while (st.req_state == REQ_POSTED && !st.stopping) {
		if (pthread_cond_timedwait(&st.cond, &st.lock, &deadline) == ETIMEDOUT) {
			break;
		}
	}
	if (st.req_state == REQ_POSTED) {
		/* Not started in time, or shutdown began: withdrawn, so the owner never serves a stale buffer. */
		st.req_state = REQ_IDLE;
		pthread_mutex_unlock(&st.lock);
		memset(buffer, 0, bytes);
		return;
	}
	while (st.req_state == REQ_STARTED) {
		pthread_cond_wait(&st.cond, &st.lock);
	}
	pthread_mutex_unlock(&st.lock);
}

void phpsaudio_stream_cb(float *buffer, int num_frames, int num_channels)
{
	stream_request(false, buffer, num_frames, num_channels);
}

void phpsaudio_stream_userdata_cb(float *buffer, int num_frames, int num_channels, void *user_data)
{
	(void)user_data; /* the PHP value lives in phpsaudio_values.user_data */
	stream_request(true, buffer, num_frames, num_channels);
}

void phpsaudio_logger(const char *tag, uint32_t log_level, uint32_t log_item_id, const char *message_or_null,
	uint32_t line_nr, const char *filename_or_null, void *user_data)
{
	(void)user_data; /* the PHP value lives in phpsaudio_values.logger_user_data */

	pthread_mutex_lock(&st.lock);
	if (st.setup && pthread_equal(st.owner, pthread_self())) {
		pthread_mutex_unlock(&st.lock);
		run_logger(tag, log_level, log_item_id, message_or_null, line_nr, filename_or_null);
		return;
	}

	phpsaudio_log *log = malloc(sizeof(*log));
	if (log) {
		log->tag = tag;
		log->log_level = log_level;
		log->log_item_id = log_item_id;
		log->message = message_or_null;
		log->line_nr = line_nr;
		log->filename = filename_or_null;
		log->next = NULL;
		if (st.logs_tail) {
			st.logs_tail->next = log;
		} else {
			st.logs_head = log;
		}
		st.logs_tail = log;
		if (st.setup && !st.dead) {
			zend_atomic_bool_store(st.owner_interrupt, true);
		}
	}
	pthread_mutex_unlock(&st.lock);
}

/* ---- shutdown, fork, lifecycle ---- */

void phpsaudio_shutdown(bool deliver)
{
	/* Wakes a waiting audio thread to silence; sokol-audio can then join it. */
	pthread_mutex_lock(&st.lock);
	st.stopping = true;
	pthread_cond_broadcast(&st.cond);
	pthread_mutex_unlock(&st.lock);

	saudio_shutdown();

	pthread_mutex_lock(&st.lock);
	st.stopping = false;
	st.setup = false;
	st.req_state = REQ_IDLE;
	phpsaudio_log *logs = take_logs_locked();
	pthread_mutex_unlock(&st.lock);

	deliver_logs(logs, deliver);
	release_held();
}

static void atfork_prepare(void)
{
	pthread_mutex_lock(&st.lock);
}

static void atfork_parent(void)
{
	pthread_mutex_unlock(&st.lock);
}

static void atfork_child(void)
{
	pthread_mutex_init(&st.lock, NULL);
	pthread_cond_init(&st.cond, NULL);
	st.req_state = REQ_IDLE;
	st.stopping = false;
	if (st.setup && saudio_isvalid()) {
		st.dead = true;
	}
}

void phpsaudio_callbacks_minit(void)
{
	ZVAL_NULL(&phpsaudio_values.stream_cb);
	ZVAL_NULL(&phpsaudio_values.stream_userdata_cb);
	ZVAL_NULL(&phpsaudio_values.user_data);
	ZVAL_NULL(&phpsaudio_values.logger_func);
	ZVAL_NULL(&phpsaudio_values.logger_user_data);
	ZVAL_NULL(&phpsaudio_values.allocator_user_data);

	prev_interrupt_function = zend_interrupt_function;
	zend_interrupt_function = interrupt_function;
	pthread_atfork(atfork_prepare, atfork_parent, atfork_child);
}

/* Request end: a sokol-audio this thread set up is shut down. Logs still queued are dropped: no script is left to act on them. */
void phpsaudio_callbacks_rshutdown(void)
{
	pthread_mutex_lock(&st.lock);
	bool mine = is_owner_locked(), dead = st.dead;
	pthread_mutex_unlock(&st.lock);

	if (!mine) {
		return;
	}
	callback_depth = 0;
	if (dead) {
		/* The audio thread is the parent's. Its state is unusable here; only the PHP values are released. */
		pthread_mutex_lock(&st.lock);
		phpsaudio_log *logs = take_logs_locked();
		pthread_mutex_unlock(&st.lock);
		deliver_logs(logs, false);
		release_held();
		return;
	}
	phpsaudio_shutdown(false);
}
