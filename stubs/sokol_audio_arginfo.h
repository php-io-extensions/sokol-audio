/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 256c9aef70be58446bc2f1360ba73a537465af9c */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_saudio_setup, 0, 1, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, desc, saudio_desc, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_saudio_shutdown, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_saudio_isvalid, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_saudio_userdata, 0, 0, IS_MIXED, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_saudio_query_desc, 0, 0, saudio_desc, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_saudio_sample_rate, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

#define arginfo_saudio_buffer_frames arginfo_saudio_sample_rate

#define arginfo_saudio_channels arginfo_saudio_sample_rate

#define arginfo_saudio_suspended arginfo_saudio_isvalid

#define arginfo_saudio_expect arginfo_saudio_sample_rate

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_saudio_push, 0, 2, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, frames, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, num_frames, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_saudio_logger___construct, 0, 0, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, func, Closure, 1, "null")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, user_data, IS_MIXED, 0, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_saudio_allocator___construct, 0, 0, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, alloc_fn, Closure, 1, "null")
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, free_fn, Closure, 1, "null")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, user_data, IS_MIXED, 0, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_saudio_n3ds_desc___construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, queue_count, IS_LONG, 0, "0")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, interpolation_type, IS_LONG, 0, "0")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, channel_id, IS_LONG, 0, "0")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_saudio_win32_desc___construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, skip_coinitialize, _IS_BOOL, 0, "false")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_saudio_desc___construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, sample_rate, IS_LONG, 0, "0")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, num_channels, IS_LONG, 0, "0")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, buffer_frames, IS_LONG, 0, "0")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, packet_frames, IS_LONG, 0, "0")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, num_packets, IS_LONG, 0, "0")
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, stream_cb, Closure, 1, "null")
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, stream_userdata_cb, Closure, 1, "null")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, user_data, IS_MIXED, 0, "null")
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, win32, saudio_win32_desc, 0, "new saudio_win32_desc()")
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, n3ds, saudio_n3ds_desc, 0, "new saudio_n3ds_desc()")
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, allocator, saudio_allocator, 0, "new saudio_allocator()")
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, logger, saudio_logger, 0, "new saudio_logger()")
ZEND_END_ARG_INFO()

ZEND_FUNCTION(saudio_setup);
ZEND_FUNCTION(saudio_shutdown);
ZEND_FUNCTION(saudio_isvalid);
ZEND_FUNCTION(saudio_userdata);
ZEND_FUNCTION(saudio_query_desc);
ZEND_FUNCTION(saudio_sample_rate);
ZEND_FUNCTION(saudio_buffer_frames);
ZEND_FUNCTION(saudio_channels);
ZEND_FUNCTION(saudio_suspended);
ZEND_FUNCTION(saudio_expect);
ZEND_FUNCTION(saudio_push);
ZEND_METHOD(saudio_logger, __construct);
ZEND_METHOD(saudio_allocator, __construct);
ZEND_METHOD(saudio_n3ds_desc, __construct);
ZEND_METHOD(saudio_win32_desc, __construct);
ZEND_METHOD(saudio_desc, __construct);

static const zend_function_entry ext_functions[] = {
	ZEND_FE(saudio_setup, arginfo_saudio_setup)
	ZEND_FE(saudio_shutdown, arginfo_saudio_shutdown)
	ZEND_FE(saudio_isvalid, arginfo_saudio_isvalid)
	ZEND_FE(saudio_userdata, arginfo_saudio_userdata)
	ZEND_FE(saudio_query_desc, arginfo_saudio_query_desc)
	ZEND_FE(saudio_sample_rate, arginfo_saudio_sample_rate)
	ZEND_FE(saudio_buffer_frames, arginfo_saudio_buffer_frames)
	ZEND_FE(saudio_channels, arginfo_saudio_channels)
	ZEND_FE(saudio_suspended, arginfo_saudio_suspended)
	ZEND_FE(saudio_expect, arginfo_saudio_expect)
	ZEND_FE(saudio_push, arginfo_saudio_push)
	ZEND_FE_END
};

static const zend_function_entry class_saudio_logger_methods[] = {
	ZEND_ME(saudio_logger, __construct, arginfo_class_saudio_logger___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_saudio_allocator_methods[] = {
	ZEND_ME(saudio_allocator, __construct, arginfo_class_saudio_allocator___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_saudio_n3ds_desc_methods[] = {
	ZEND_ME(saudio_n3ds_desc, __construct, arginfo_class_saudio_n3ds_desc___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_saudio_win32_desc_methods[] = {
	ZEND_ME(saudio_win32_desc, __construct, arginfo_class_saudio_win32_desc___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_saudio_desc_methods[] = {
	ZEND_ME(saudio_desc, __construct, arginfo_class_saudio_desc___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static void register_sokol_audio_symbols(int module_number)
{
	REGISTER_LONG_CONSTANT("SAUDIO_LOGITEM_OK", SAUDIO_LOGITEM_OK, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SAUDIO_LOGITEM_MALLOC_FAILED", SAUDIO_LOGITEM_MALLOC_FAILED, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SAUDIO_LOGITEM_ALSA_SND_PCM_OPEN_FAILED", SAUDIO_LOGITEM_ALSA_SND_PCM_OPEN_FAILED, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SAUDIO_LOGITEM_ALSA_FLOAT_SAMPLES_NOT_SUPPORTED", SAUDIO_LOGITEM_ALSA_FLOAT_SAMPLES_NOT_SUPPORTED, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SAUDIO_LOGITEM_ALSA_REQUESTED_BUFFER_SIZE_NOT_SUPPORTED", SAUDIO_LOGITEM_ALSA_REQUESTED_BUFFER_SIZE_NOT_SUPPORTED, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SAUDIO_LOGITEM_ALSA_REQUESTED_CHANNEL_COUNT_NOT_SUPPORTED", SAUDIO_LOGITEM_ALSA_REQUESTED_CHANNEL_COUNT_NOT_SUPPORTED, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SAUDIO_LOGITEM_ALSA_SND_PCM_HW_PARAMS_SET_RATE_NEAR_FAILED", SAUDIO_LOGITEM_ALSA_SND_PCM_HW_PARAMS_SET_RATE_NEAR_FAILED, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SAUDIO_LOGITEM_ALSA_SND_PCM_HW_PARAMS_FAILED", SAUDIO_LOGITEM_ALSA_SND_PCM_HW_PARAMS_FAILED, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SAUDIO_LOGITEM_ALSA_PTHREAD_CREATE_FAILED", SAUDIO_LOGITEM_ALSA_PTHREAD_CREATE_FAILED, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SAUDIO_LOGITEM_PULSEAUDIO_INVALID_SAMPLE_SPEC", SAUDIO_LOGITEM_PULSEAUDIO_INVALID_SAMPLE_SPEC, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SAUDIO_LOGITEM_PULSEAUDIO_SIMPLE_NEW_FAILED", SAUDIO_LOGITEM_PULSEAUDIO_SIMPLE_NEW_FAILED, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SAUDIO_LOGITEM_PULSEAUDIO_SIMPLE_WRITE_FAILED", SAUDIO_LOGITEM_PULSEAUDIO_SIMPLE_WRITE_FAILED, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SAUDIO_LOGITEM_PULSEAUDIO_PTHREAD_CREATE_FAILED", SAUDIO_LOGITEM_PULSEAUDIO_PTHREAD_CREATE_FAILED, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SAUDIO_LOGITEM_WASAPI_CREATE_EVENT_FAILED", SAUDIO_LOGITEM_WASAPI_CREATE_EVENT_FAILED, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SAUDIO_LOGITEM_WASAPI_CREATE_DEVICE_ENUMERATOR_FAILED", SAUDIO_LOGITEM_WASAPI_CREATE_DEVICE_ENUMERATOR_FAILED, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SAUDIO_LOGITEM_WASAPI_GET_DEFAULT_AUDIO_ENDPOINT_FAILED", SAUDIO_LOGITEM_WASAPI_GET_DEFAULT_AUDIO_ENDPOINT_FAILED, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SAUDIO_LOGITEM_WASAPI_DEVICE_ACTIVATE_FAILED", SAUDIO_LOGITEM_WASAPI_DEVICE_ACTIVATE_FAILED, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SAUDIO_LOGITEM_WASAPI_AUDIO_CLIENT_INITIALIZE_FAILED", SAUDIO_LOGITEM_WASAPI_AUDIO_CLIENT_INITIALIZE_FAILED, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SAUDIO_LOGITEM_WASAPI_AUDIO_CLIENT_GET_BUFFER_SIZE_FAILED", SAUDIO_LOGITEM_WASAPI_AUDIO_CLIENT_GET_BUFFER_SIZE_FAILED, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SAUDIO_LOGITEM_WASAPI_AUDIO_CLIENT_GET_SERVICE_FAILED", SAUDIO_LOGITEM_WASAPI_AUDIO_CLIENT_GET_SERVICE_FAILED, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SAUDIO_LOGITEM_WASAPI_AUDIO_CLIENT_SET_EVENT_HANDLE_FAILED", SAUDIO_LOGITEM_WASAPI_AUDIO_CLIENT_SET_EVENT_HANDLE_FAILED, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SAUDIO_LOGITEM_WASAPI_CREATE_THREAD_FAILED", SAUDIO_LOGITEM_WASAPI_CREATE_THREAD_FAILED, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SAUDIO_LOGITEM_AAUDIO_STREAMBUILDER_OPEN_STREAM_FAILED", SAUDIO_LOGITEM_AAUDIO_STREAMBUILDER_OPEN_STREAM_FAILED, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SAUDIO_LOGITEM_AAUDIO_PTHREAD_CREATE_FAILED", SAUDIO_LOGITEM_AAUDIO_PTHREAD_CREATE_FAILED, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SAUDIO_LOGITEM_AAUDIO_RESTARTING_STREAM_AFTER_ERROR", SAUDIO_LOGITEM_AAUDIO_RESTARTING_STREAM_AFTER_ERROR, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SAUDIO_LOGITEM_USING_AAUDIO_BACKEND", SAUDIO_LOGITEM_USING_AAUDIO_BACKEND, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SAUDIO_LOGITEM_AAUDIO_CREATE_STREAMBUILDER_FAILED", SAUDIO_LOGITEM_AAUDIO_CREATE_STREAMBUILDER_FAILED, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SAUDIO_LOGITEM_COREAUDIO_NEW_OUTPUT_FAILED", SAUDIO_LOGITEM_COREAUDIO_NEW_OUTPUT_FAILED, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SAUDIO_LOGITEM_COREAUDIO_ALLOCATE_BUFFER_FAILED", SAUDIO_LOGITEM_COREAUDIO_ALLOCATE_BUFFER_FAILED, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SAUDIO_LOGITEM_COREAUDIO_START_FAILED", SAUDIO_LOGITEM_COREAUDIO_START_FAILED, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SAUDIO_LOGITEM_BACKEND_BUFFER_SIZE_ISNT_MULTIPLE_OF_PACKET_SIZE", SAUDIO_LOGITEM_BACKEND_BUFFER_SIZE_ISNT_MULTIPLE_OF_PACKET_SIZE, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SAUDIO_LOGITEM_VITA_SCEAUDIO_OPEN_FAILED", SAUDIO_LOGITEM_VITA_SCEAUDIO_OPEN_FAILED, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SAUDIO_LOGITEM_VITA_PTHREAD_CREATE_FAILED", SAUDIO_LOGITEM_VITA_PTHREAD_CREATE_FAILED, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SAUDIO_LOGITEM_N3DS_NDSP_OPEN_FAILED", SAUDIO_LOGITEM_N3DS_NDSP_OPEN_FAILED, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SAUDIO_N3DS_DSP_INTERP_POLYPHASE", SAUDIO_N3DS_DSP_INTERP_POLYPHASE, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SAUDIO_N3DS_DSP_INTERP_LINEAR", SAUDIO_N3DS_DSP_INTERP_LINEAR, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SAUDIO_N3DS_DSP_INTERP_NONE", SAUDIO_N3DS_DSP_INTERP_NONE, CONST_PERSISTENT);
}

static zend_class_entry *register_class_saudio_logger(void)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "saudio_logger", class_saudio_logger_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	zval property_func_default_value;
	ZVAL_NULL(&property_func_default_value);
	zend_string *property_func_name = zend_string_init("func", sizeof("func") - 1, 1);
	zend_string *property_func_class_Closure = zend_string_init("Closure", sizeof("Closure")-1, 1);
	zend_declare_typed_property(class_entry, property_func_name, &property_func_default_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_func_class_Closure, 0, MAY_BE_NULL));
	zend_string_release(property_func_name);

	zval property_user_data_default_value;
	ZVAL_NULL(&property_user_data_default_value);
	zend_string *property_user_data_name = zend_string_init("user_data", sizeof("user_data") - 1, 1);
	zend_declare_typed_property(class_entry, property_user_data_name, &property_user_data_default_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_ANY));
	zend_string_release(property_user_data_name);

	return class_entry;
}

static zend_class_entry *register_class_saudio_allocator(void)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "saudio_allocator", class_saudio_allocator_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	zval property_alloc_fn_default_value;
	ZVAL_NULL(&property_alloc_fn_default_value);
	zend_string *property_alloc_fn_name = zend_string_init("alloc_fn", sizeof("alloc_fn") - 1, 1);
	zend_string *property_alloc_fn_class_Closure = zend_string_init("Closure", sizeof("Closure")-1, 1);
	zend_declare_typed_property(class_entry, property_alloc_fn_name, &property_alloc_fn_default_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_alloc_fn_class_Closure, 0, MAY_BE_NULL));
	zend_string_release(property_alloc_fn_name);

	zval property_free_fn_default_value;
	ZVAL_NULL(&property_free_fn_default_value);
	zend_string *property_free_fn_name = zend_string_init("free_fn", sizeof("free_fn") - 1, 1);
	zend_string *property_free_fn_class_Closure = zend_string_init("Closure", sizeof("Closure")-1, 1);
	zend_declare_typed_property(class_entry, property_free_fn_name, &property_free_fn_default_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_free_fn_class_Closure, 0, MAY_BE_NULL));
	zend_string_release(property_free_fn_name);

	zval property_user_data_default_value;
	ZVAL_NULL(&property_user_data_default_value);
	zend_string *property_user_data_name = zend_string_init("user_data", sizeof("user_data") - 1, 1);
	zend_declare_typed_property(class_entry, property_user_data_name, &property_user_data_default_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_ANY));
	zend_string_release(property_user_data_name);

	return class_entry;
}

static zend_class_entry *register_class_saudio_n3ds_desc(void)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "saudio_n3ds_desc", class_saudio_n3ds_desc_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	zval property_queue_count_default_value;
	ZVAL_LONG(&property_queue_count_default_value, 0);
	zend_string *property_queue_count_name = zend_string_init("queue_count", sizeof("queue_count") - 1, 1);
	zend_declare_typed_property(class_entry, property_queue_count_name, &property_queue_count_default_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(property_queue_count_name);

	zval property_interpolation_type_default_value;
	ZVAL_LONG(&property_interpolation_type_default_value, 0);
	zend_string *property_interpolation_type_name = zend_string_init("interpolation_type", sizeof("interpolation_type") - 1, 1);
	zend_declare_typed_property(class_entry, property_interpolation_type_name, &property_interpolation_type_default_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(property_interpolation_type_name);

	zval property_channel_id_default_value;
	ZVAL_LONG(&property_channel_id_default_value, 0);
	zend_string *property_channel_id_name = zend_string_init("channel_id", sizeof("channel_id") - 1, 1);
	zend_declare_typed_property(class_entry, property_channel_id_name, &property_channel_id_default_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(property_channel_id_name);

	return class_entry;
}

static zend_class_entry *register_class_saudio_win32_desc(void)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "saudio_win32_desc", class_saudio_win32_desc_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	zval property_skip_coinitialize_default_value;
	ZVAL_FALSE(&property_skip_coinitialize_default_value);
	zend_string *property_skip_coinitialize_name = zend_string_init("skip_coinitialize", sizeof("skip_coinitialize") - 1, 1);
	zend_declare_typed_property(class_entry, property_skip_coinitialize_name, &property_skip_coinitialize_default_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_BOOL));
	zend_string_release(property_skip_coinitialize_name);

	return class_entry;
}

static zend_class_entry *register_class_saudio_desc(void)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "saudio_desc", class_saudio_desc_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	zval property_sample_rate_default_value;
	ZVAL_LONG(&property_sample_rate_default_value, 0);
	zend_string *property_sample_rate_name = zend_string_init("sample_rate", sizeof("sample_rate") - 1, 1);
	zend_declare_typed_property(class_entry, property_sample_rate_name, &property_sample_rate_default_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(property_sample_rate_name);

	zval property_num_channels_default_value;
	ZVAL_LONG(&property_num_channels_default_value, 0);
	zend_string *property_num_channels_name = zend_string_init("num_channels", sizeof("num_channels") - 1, 1);
	zend_declare_typed_property(class_entry, property_num_channels_name, &property_num_channels_default_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(property_num_channels_name);

	zval property_buffer_frames_default_value;
	ZVAL_LONG(&property_buffer_frames_default_value, 0);
	zend_string *property_buffer_frames_name = zend_string_init("buffer_frames", sizeof("buffer_frames") - 1, 1);
	zend_declare_typed_property(class_entry, property_buffer_frames_name, &property_buffer_frames_default_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(property_buffer_frames_name);

	zval property_packet_frames_default_value;
	ZVAL_LONG(&property_packet_frames_default_value, 0);
	zend_string *property_packet_frames_name = zend_string_init("packet_frames", sizeof("packet_frames") - 1, 1);
	zend_declare_typed_property(class_entry, property_packet_frames_name, &property_packet_frames_default_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(property_packet_frames_name);

	zval property_num_packets_default_value;
	ZVAL_LONG(&property_num_packets_default_value, 0);
	zend_string *property_num_packets_name = zend_string_init("num_packets", sizeof("num_packets") - 1, 1);
	zend_declare_typed_property(class_entry, property_num_packets_name, &property_num_packets_default_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(property_num_packets_name);

	zval property_stream_cb_default_value;
	ZVAL_NULL(&property_stream_cb_default_value);
	zend_string *property_stream_cb_name = zend_string_init("stream_cb", sizeof("stream_cb") - 1, 1);
	zend_string *property_stream_cb_class_Closure = zend_string_init("Closure", sizeof("Closure")-1, 1);
	zend_declare_typed_property(class_entry, property_stream_cb_name, &property_stream_cb_default_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_stream_cb_class_Closure, 0, MAY_BE_NULL));
	zend_string_release(property_stream_cb_name);

	zval property_stream_userdata_cb_default_value;
	ZVAL_NULL(&property_stream_userdata_cb_default_value);
	zend_string *property_stream_userdata_cb_name = zend_string_init("stream_userdata_cb", sizeof("stream_userdata_cb") - 1, 1);
	zend_string *property_stream_userdata_cb_class_Closure = zend_string_init("Closure", sizeof("Closure")-1, 1);
	zend_declare_typed_property(class_entry, property_stream_userdata_cb_name, &property_stream_userdata_cb_default_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_stream_userdata_cb_class_Closure, 0, MAY_BE_NULL));
	zend_string_release(property_stream_userdata_cb_name);

	zval property_user_data_default_value;
	ZVAL_NULL(&property_user_data_default_value);
	zend_string *property_user_data_name = zend_string_init("user_data", sizeof("user_data") - 1, 1);
	zend_declare_typed_property(class_entry, property_user_data_name, &property_user_data_default_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_ANY));
	zend_string_release(property_user_data_name);

	zval property_win32_default_value;
	ZVAL_UNDEF(&property_win32_default_value);
	zend_string *property_win32_name = zend_string_init("win32", sizeof("win32") - 1, 1);
	zend_string *property_win32_class_saudio_win32_desc = zend_string_init("saudio_win32_desc", sizeof("saudio_win32_desc")-1, 1);
	zend_declare_typed_property(class_entry, property_win32_name, &property_win32_default_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_win32_class_saudio_win32_desc, 0, 0));
	zend_string_release(property_win32_name);

	zval property_n3ds_default_value;
	ZVAL_UNDEF(&property_n3ds_default_value);
	zend_string *property_n3ds_name = zend_string_init("n3ds", sizeof("n3ds") - 1, 1);
	zend_string *property_n3ds_class_saudio_n3ds_desc = zend_string_init("saudio_n3ds_desc", sizeof("saudio_n3ds_desc")-1, 1);
	zend_declare_typed_property(class_entry, property_n3ds_name, &property_n3ds_default_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_n3ds_class_saudio_n3ds_desc, 0, 0));
	zend_string_release(property_n3ds_name);

	zval property_allocator_default_value;
	ZVAL_UNDEF(&property_allocator_default_value);
	zend_string *property_allocator_name = zend_string_init("allocator", sizeof("allocator") - 1, 1);
	zend_string *property_allocator_class_saudio_allocator = zend_string_init("saudio_allocator", sizeof("saudio_allocator")-1, 1);
	zend_declare_typed_property(class_entry, property_allocator_name, &property_allocator_default_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_allocator_class_saudio_allocator, 0, 0));
	zend_string_release(property_allocator_name);

	zval property_logger_default_value;
	ZVAL_UNDEF(&property_logger_default_value);
	zend_string *property_logger_name = zend_string_init("logger", sizeof("logger") - 1, 1);
	zend_string *property_logger_class_saudio_logger = zend_string_init("saudio_logger", sizeof("saudio_logger")-1, 1);
	zend_declare_typed_property(class_entry, property_logger_name, &property_logger_default_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_logger_class_saudio_logger, 0, 0));
	zend_string_release(property_logger_name);

	return class_entry;
}

static zend_class_entry *register_class_saudio_assertion_error(zend_class_entry *class_entry_Error)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "saudio_assertion_error", NULL);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_Error, ZEND_ACC_FINAL);

	return class_entry;
}
