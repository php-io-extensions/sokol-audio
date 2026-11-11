#ifndef PHPSAUDIO_BINDING_H
#define PHPSAUDIO_BINDING_H

#include "php.h"
#include "runtime.h"
#include "sokol_audio.h"

#include <stdbool.h>

extern zend_class_entry *phpsaudio_ce_assertion_error;

/* PHP values saudio_setup() handed to sokol-audio as C callbacks or user data.
 * Owned by the thread that called saudio_setup(); only that thread touches them. */
typedef struct phpsaudio_held {
	zval stream_cb;
	zval stream_userdata_cb;
	zval user_data;
	zval logger_func;
	zval logger_user_data;
	zval allocator_user_data;
} phpsaudio_held;

extern phpsaudio_held phpsaudio_values;

/* Guards. Each throws and returns FAILURE when the call may not reach sokol-audio. */
zend_result phpsaudio_enter(void);         /* set up, by this thread, alive */
zend_result phpsaudio_enter_setup(void);   /* not set up; claims ownership for this thread */
zend_result phpsaudio_enter_shutdown(void);

/* saudio_setup() failed validation after phpsaudio_enter_setup(): give ownership back. */
void phpsaudio_release_setup(void);
/* Upstream saudio_setup() returned: the actual sample rate sets the stream callback deadline. */
void phpsaudio_setup_done(int sample_rate);
/* Upstream saudio_shutdown() wrapped with the callback stop and the delivery of queued logs. */
void phpsaudio_shutdown(bool deliver_logs);

/* Throws saudio_assertion_error when a SOKOL_ASSERT failed since the last check. */
void phpsaudio_assert_check(void);

void phpsaudio_callbacks_minit(void);
void phpsaudio_callbacks_rshutdown(void);

#endif
