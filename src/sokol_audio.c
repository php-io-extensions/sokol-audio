/*
 * sokol_audio: 1:1 bindings of floooh/sokol's sokol_audio.h. The library is
 * compiled into this extension. Function, struct, enum and constant names
 * stay the C names.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_sokol_audio.h"

#include "ext/standard/info.h"
#include "zend_exceptions.h"

#include "binding.h"

#include "../stubs/sokol_audio_arginfo.h"

#include <limits.h>

static zend_class_entry *ce_logger, *ce_allocator, *ce_n3ds_desc, *ce_win32_desc, *ce_desc;
static zend_object_handlers value_handlers;

/* ---- struct classes: plain mutable values, clone copies nested structs ---- */

static bool is_struct_class(zend_class_entry *ce)
{
	return ce == ce_logger || ce == ce_allocator || ce == ce_n3ds_desc || ce == ce_win32_desc || ce == ce_desc;
}

static void set_new(zend_object *obj, const char *name, zend_class_entry *ce)
{
	zval child;

	object_init_ex(&child, ce);
	zend_update_property(obj->ce, obj, name, strlen(name), &child);
	zval_ptr_dtor(&child);
}

static zend_object *value_create(zend_class_entry *ce)
{
	zend_object *obj = zend_objects_new(ce);

	object_properties_init(obj, ce);
	obj->handlers = &value_handlers;
	if (ce == ce_desc) {
		/* The C aggregate initializer: nested structs exist, zeroed. */
		set_new(obj, "win32", ce_win32_desc);
		set_new(obj, "n3ds", ce_n3ds_desc);
		set_new(obj, "allocator", ce_allocator);
		set_new(obj, "logger", ce_logger);
	}
	return obj;
}

static zend_object *value_clone(zend_object *old)
{
	zend_object *clone = zend_objects_new(old->ce);

	object_properties_init(clone, old->ce);
	clone->handlers = &value_handlers;
	zend_objects_clone_members(clone, old);

	zend_property_info *info;
	ZEND_HASH_FOREACH_PTR(&clone->ce->properties_info, info) {
		zval *slot = OBJ_PROP(clone, info->offset);
		if (Z_TYPE_P(slot) == IS_OBJECT && is_struct_class(Z_OBJCE_P(slot))) {
			zend_object *copy = value_clone(Z_OBJ_P(slot));
			zval_ptr_dtor(slot);
			ZVAL_OBJ(slot, copy);
		}
	} ZEND_HASH_FOREACH_END();
	return clone;
}

/* Constructor shared by every struct class: one optional argument per field, in field order. */
static void value_construct(INTERNAL_FUNCTION_PARAMETERS)
{
	zval *args = NULL;
	uint32_t argc = 0;
	zend_object *obj = Z_OBJ_P(ZEND_THIS);
	uint32_t fields = zend_hash_num_elements(&obj->ce->properties_info);

	ZEND_PARSE_PARAMETERS_START(0, -1)
		Z_PARAM_VARIADIC('*', args, argc)
	ZEND_PARSE_PARAMETERS_END();

	if (argc > fields) {
		zend_argument_count_error("%s::__construct() expects at most %u arguments, %u given",
			ZSTR_VAL(obj->ce->name), fields, argc);
		RETURN_THROWS();
	}

	uint32_t i = 0;
	zend_property_info *info;
	ZEND_HASH_FOREACH_PTR(&obj->ce->properties_info, info) {
		if (i >= argc) {
			break;
		}
		if (Z_TYPE(args[i]) != IS_UNDEF) {
			zend_update_property_ex(obj->ce, obj, info->name, &args[i]);
			if (EG(exception)) {
				RETURN_THROWS();
			}
		}
		i++;
	} ZEND_HASH_FOREACH_END();
}

ZEND_METHOD(saudio_logger, __construct) { value_construct(INTERNAL_FUNCTION_PARAM_PASSTHRU); }
ZEND_METHOD(saudio_allocator, __construct) { value_construct(INTERNAL_FUNCTION_PARAM_PASSTHRU); }
ZEND_METHOD(saudio_n3ds_desc, __construct) { value_construct(INTERNAL_FUNCTION_PARAM_PASSTHRU); }
ZEND_METHOD(saudio_win32_desc, __construct) { value_construct(INTERNAL_FUNCTION_PARAM_PASSTHRU); }
ZEND_METHOD(saudio_desc, __construct) { value_construct(INTERNAL_FUNCTION_PARAM_PASSTHRU); }

/* ---- conversions ---- */

/* Reads a typed property; an unset one throws "must not be accessed before initialization". */
static zval *field(zend_object *obj, const char *name, zval *rv)
{
	zval *v = zend_read_property(obj->ce, obj, name, strlen(name), false, rv);
	return EG(exception) ? NULL : v;
}

static zend_result int_field(zend_object *obj, const char *name, zend_long min, int *out)
{
	zval rv, *v = field(obj, name, &rv);

	if (!v) {
		return FAILURE;
	}
	if (Z_LVAL_P(v) < min || Z_LVAL_P(v) > INT_MAX) {
		zend_value_error("%s::$%s must be between " ZEND_LONG_FMT " and %d, " ZEND_LONG_FMT " given",
			ZSTR_VAL(obj->ce->name), name, min, INT_MAX, Z_LVAL_P(v));
		return FAILURE;
	}
	*out = (int)Z_LVAL_P(v);
	return SUCCESS;
}

static zend_object *struct_field(zend_object *obj, const char *name)
{
	zval rv, *v = field(obj, name, &rv);
	return v ? Z_OBJ_P(v) : NULL;
}

static int def(int value, int fallback)
{
	return value == 0 ? fallback : value;
}

/* saudio_desc → C. PHP closures and user data go to phpsaudio_values; sokol-audio gets the C trampolines. */
static zend_result desc_from_object(zend_object *obj, saudio_desc *desc, phpsaudio_held *held)
{
	zval rv, *v;
	zend_object *win32, *n3ds, *allocator, *logger;
	int interpolation_type;

	memset(desc, 0, sizeof(*desc));

	if (int_field(obj, "sample_rate", 0, &desc->sample_rate) == FAILURE
		|| int_field(obj, "num_channels", 0, &desc->num_channels) == FAILURE
		|| int_field(obj, "buffer_frames", 0, &desc->buffer_frames) == FAILURE
		|| int_field(obj, "packet_frames", 0, &desc->packet_frames) == FAILURE
		|| int_field(obj, "num_packets", 0, &desc->num_packets) == FAILURE) {
		return FAILURE;
	}

	/* The sizes sokol-audio derives must not trip its own asserts or overflow its int byte counts. */
	int64_t channels = def(desc->num_channels, 1);
	int64_t buffer_frames = def(desc->buffer_frames, phpsaudio_default_buffer_frames);
	int64_t packet_frames = def(desc->packet_frames, phpsaudio_default_packet_frames);
	int64_t num_packets = def(desc->num_packets, phpsaudio_default_num_packets);
	int64_t bytes_per_frame = channels * (int64_t)sizeof(float);
	if (num_packets + 1 > phpsaudio_ring_max_slots) {
		zend_value_error("saudio_desc::$num_packets must be at most %d (SAUDIO_RING_MAX_SLOTS - 1), " ZEND_LONG_FMT " given",
			phpsaudio_ring_max_slots - 1, (zend_long)num_packets);
		return FAILURE;
	}
	if (buffer_frames * bytes_per_frame > INT_MAX) {
		zend_value_error("saudio_desc::$buffer_frames x $num_channels x 4 must be at most %d bytes", INT_MAX);
		return FAILURE;
	}
	if (packet_frames * bytes_per_frame * num_packets > INT_MAX) {
		zend_value_error("saudio_desc::$packet_frames x $num_channels x 4 x $num_packets must be at most %d bytes", INT_MAX);
		return FAILURE;
	}

	if (!(v = field(obj, "stream_cb", &rv))) {
		return FAILURE;
	}
	ZVAL_COPY(&held->stream_cb, v);
	if (!(v = field(obj, "stream_userdata_cb", &rv))) {
		return FAILURE;
	}
	ZVAL_COPY(&held->stream_userdata_cb, v);
	if (!(v = field(obj, "user_data", &rv))) {
		return FAILURE;
	}
	ZVAL_COPY(&held->user_data, v);
	desc->stream_cb = Z_TYPE(held->stream_cb) == IS_OBJECT ? phpsaudio_stream_cb : NULL;
	desc->stream_userdata_cb = Z_TYPE(held->stream_userdata_cb) == IS_OBJECT ? phpsaudio_stream_userdata_cb : NULL;
	desc->user_data = NULL;

	if (!(win32 = struct_field(obj, "win32")) || !(v = field(win32, "skip_coinitialize", &rv))) {
		return FAILURE;
	}
	desc->win32.skip_coinitialize = Z_TYPE_P(v) == IS_TRUE;

	if (!(n3ds = struct_field(obj, "n3ds"))
		|| int_field(n3ds, "queue_count", INT_MIN, &desc->n3ds.queue_count) == FAILURE
		|| int_field(n3ds, "interpolation_type", INT_MIN, &interpolation_type) == FAILURE
		|| int_field(n3ds, "channel_id", INT_MIN, &desc->n3ds.channel_id) == FAILURE) {
		return FAILURE;
	}
	desc->n3ds.interpolation_type = (saudio_n3ds_ndspinterptype)interpolation_type;

	if (!(allocator = struct_field(obj, "allocator"))) {
		return FAILURE;
	}
	for (int i = 0; i < 2; i++) {
		const char *name = i == 0 ? "alloc_fn" : "free_fn";
		if (!(v = field(allocator, name, &rv))) {
			return FAILURE;
		}
		if (Z_TYPE_P(v) != IS_NULL) {
			zend_value_error("saudio_allocator::$%s must be null: PHP has no memory to hand sokol-audio, which keeps malloc and free", name);
			return FAILURE;
		}
	}
	if (!(v = field(allocator, "user_data", &rv))) {
		return FAILURE;
	}
	ZVAL_COPY(&held->allocator_user_data, v);

	if (!(logger = struct_field(obj, "logger")) || !(v = field(logger, "func", &rv))) {
		return FAILURE;
	}
	ZVAL_COPY(&held->logger_func, v);
	if (!(v = field(logger, "user_data", &rv))) {
		return FAILURE;
	}
	ZVAL_COPY(&held->logger_user_data, v);
	desc->logger.func = Z_TYPE(held->logger_func) == IS_OBJECT ? phpsaudio_logger : NULL;

	return SUCCESS;
}

static void update_long(zend_object *obj, const char *name, zend_long value)
{
	zend_update_property_long(obj->ce, obj, name, strlen(name), value);
}

static void update_zval(zend_object *obj, const char *name, zval *value)
{
	zend_update_property(obj->ce, obj, name, strlen(name), value);
}

static void desc_to_zval(const saudio_desc *desc, zval *out)
{
	zval rv;

	object_init_ex(out, ce_desc);
	zend_object *obj = Z_OBJ_P(out);

	update_long(obj, "sample_rate", desc->sample_rate);
	update_long(obj, "num_channels", desc->num_channels);
	update_long(obj, "buffer_frames", desc->buffer_frames);
	update_long(obj, "packet_frames", desc->packet_frames);
	update_long(obj, "num_packets", desc->num_packets);
	update_zval(obj, "stream_cb", &phpsaudio_values.stream_cb);
	update_zval(obj, "stream_userdata_cb", &phpsaudio_values.stream_userdata_cb);
	update_zval(obj, "user_data", &phpsaudio_values.user_data);

	zend_object *win32 = Z_OBJ_P(zend_read_property(ce_desc, obj, ZEND_STRL("win32"), true, &rv));
	zend_update_property_bool(ce_win32_desc, win32, ZEND_STRL("skip_coinitialize"), desc->win32.skip_coinitialize);

	zend_object *n3ds = Z_OBJ_P(zend_read_property(ce_desc, obj, ZEND_STRL("n3ds"), true, &rv));
	update_long(n3ds, "queue_count", desc->n3ds.queue_count);
	update_long(n3ds, "interpolation_type", desc->n3ds.interpolation_type);
	update_long(n3ds, "channel_id", desc->n3ds.channel_id);

	zend_object *allocator = Z_OBJ_P(zend_read_property(ce_desc, obj, ZEND_STRL("allocator"), true, &rv));
	update_zval(allocator, "user_data", &phpsaudio_values.allocator_user_data);

	zend_object *logger = Z_OBJ_P(zend_read_property(ce_desc, obj, ZEND_STRL("logger"), true, &rv));
	update_zval(logger, "func", &phpsaudio_values.logger_func);
	update_zval(logger, "user_data", &phpsaudio_values.logger_user_data);
}

/* ---- functions ---- */

ZEND_FUNCTION(saudio_setup)
{
	zval *zdesc;
	saudio_desc desc;
	phpsaudio_held held;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(zdesc, ce_desc)
	ZEND_PARSE_PARAMETERS_END();

	if (phpsaudio_enter_setup() == FAILURE) {
		RETURN_THROWS();
	}

	ZVAL_NULL(&held.stream_cb);
	ZVAL_NULL(&held.stream_userdata_cb);
	ZVAL_NULL(&held.user_data);
	ZVAL_NULL(&held.logger_func);
	ZVAL_NULL(&held.logger_user_data);
	ZVAL_NULL(&held.allocator_user_data);
	if (desc_from_object(Z_OBJ_P(zdesc), &desc, &held) == FAILURE) {
		zval_ptr_dtor(&held.stream_cb);
		zval_ptr_dtor(&held.stream_userdata_cb);
		zval_ptr_dtor(&held.user_data);
		zval_ptr_dtor(&held.logger_func);
		zval_ptr_dtor(&held.logger_user_data);
		zval_ptr_dtor(&held.allocator_user_data);
		phpsaudio_release_setup();
		RETURN_THROWS();
	}
	/* In place before sokol-audio can call back: the audio thread starts inside saudio_setup(). */
	phpsaudio_values = held;

	saudio_setup(&desc);
	phpsaudio_setup_done(saudio_sample_rate());
	phpsaudio_assert_check();
}

ZEND_FUNCTION(saudio_shutdown)
{
	ZEND_PARSE_PARAMETERS_NONE();

	if (phpsaudio_enter_shutdown() == FAILURE) {
		RETURN_THROWS();
	}
	phpsaudio_shutdown(true);
	phpsaudio_assert_check();
}

ZEND_FUNCTION(saudio_isvalid)
{
	ZEND_PARSE_PARAMETERS_NONE();

	RETVAL_BOOL(saudio_isvalid());
	phpsaudio_assert_check();
}

ZEND_FUNCTION(saudio_userdata)
{
	ZEND_PARSE_PARAMETERS_NONE();

	if (phpsaudio_enter() == FAILURE) {
		RETURN_THROWS();
	}
	(void)saudio_userdata();
	RETVAL_COPY(&phpsaudio_values.user_data);
	phpsaudio_assert_check();
}

ZEND_FUNCTION(saudio_query_desc)
{
	ZEND_PARSE_PARAMETERS_NONE();

	if (phpsaudio_enter() == FAILURE) {
		RETURN_THROWS();
	}
	saudio_desc desc = saudio_query_desc();
	desc_to_zval(&desc, return_value);
	phpsaudio_assert_check();
}

#define PHPSAUDIO_INT_GETTER(name) \
	ZEND_FUNCTION(name) \
	{ \
		ZEND_PARSE_PARAMETERS_NONE(); \
		if (phpsaudio_enter() == FAILURE) { \
			RETURN_THROWS(); \
		} \
		RETVAL_LONG(name()); \
		phpsaudio_assert_check(); \
	}

PHPSAUDIO_INT_GETTER(saudio_sample_rate)
PHPSAUDIO_INT_GETTER(saudio_buffer_frames)
PHPSAUDIO_INT_GETTER(saudio_channels)
PHPSAUDIO_INT_GETTER(saudio_expect)

ZEND_FUNCTION(saudio_suspended)
{
	ZEND_PARSE_PARAMETERS_NONE();

	if (phpsaudio_enter() == FAILURE) {
		RETURN_THROWS();
	}
	RETVAL_BOOL(saudio_suspended());
	phpsaudio_assert_check();
}

ZEND_FUNCTION(saudio_push)
{
	zend_string *frames;
	zend_long num_frames;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(frames)
		Z_PARAM_LONG(num_frames)
	ZEND_PARSE_PARAMETERS_END();

	if (phpsaudio_enter() == FAILURE) {
		RETURN_THROWS();
	}
	if (num_frames <= 0 || num_frames > INT_MAX) {
		zend_argument_value_error(2, "must be between 1 and %d", INT_MAX);
		RETURN_THROWS();
	}
	size_t needed = (size_t)num_frames * (size_t)saudio_channels() * sizeof(float);
	if (ZSTR_LEN(frames) < needed) {
		zend_argument_value_error(1, "must hold %zu bytes (" ZEND_LONG_FMT " frames x %d channels x 4), %zu given",
			needed, num_frames, saudio_channels(), ZSTR_LEN(frames));
		RETURN_THROWS();
	}

	RETVAL_LONG(saudio_push((const float *)ZSTR_VAL(frames), (int)num_frames));
	phpsaudio_assert_check();
}

/* ---- module ---- */

PHP_MINIT_FUNCTION(sokol_audio)
{
#if defined(COMPILE_DL_SOKOL_AUDIO) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	memcpy(&value_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	value_handlers.clone_obj = value_clone;

	ce_logger = register_class_saudio_logger();
	ce_allocator = register_class_saudio_allocator();
	ce_n3ds_desc = register_class_saudio_n3ds_desc();
	ce_win32_desc = register_class_saudio_win32_desc();
	ce_desc = register_class_saudio_desc();
	ce_logger->create_object = value_create;
	ce_allocator->create_object = value_create;
	ce_n3ds_desc->create_object = value_create;
	ce_win32_desc->create_object = value_create;
	ce_desc->create_object = value_create;
	phpsaudio_ce_assertion_error = register_class_saudio_assertion_error(zend_ce_error);

	register_sokol_audio_symbols(module_number);
	phpsaudio_callbacks_minit();

	return SUCCESS;
}

PHP_RINIT_FUNCTION(sokol_audio)
{
#if defined(COMPILE_DL_SOKOL_AUDIO) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(sokol_audio)
{
	phpsaudio_callbacks_rshutdown();
	return SUCCESS;
}

PHP_MINFO_FUNCTION(sokol_audio)
{
	php_info_print_table_start();
	php_info_print_table_row(2, "sokol_audio support", "enabled");
	php_info_print_table_row(2, "Version", PHP_SOKOL_AUDIO_VERSION);
	php_info_print_table_row(2, "sokol commit", PHP_SOKOL_AUDIO_UPSTREAM_COMMIT);
#if defined(__APPLE__)
	php_info_print_table_row(2, "Backend", "CoreAudio");
#else
	php_info_print_table_row(2, "Backend", "PulseAudio");
#endif
	php_info_print_table_end();
}

zend_module_entry sokol_audio_module_entry = {
	STANDARD_MODULE_HEADER,
	"sokol_audio",
	ext_functions,
	PHP_MINIT(sokol_audio),
	NULL,
	PHP_RINIT(sokol_audio),
	PHP_RSHUTDOWN(sokol_audio),
	PHP_MINFO(sokol_audio),
	PHP_SOKOL_AUDIO_VERSION,
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_SOKOL_AUDIO
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(sokol_audio)
#endif
