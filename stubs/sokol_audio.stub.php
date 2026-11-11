<?php

/** @generate-class-entries */

/* saudio_log_item: the SAUDIO_LOGITEM_* x-macro list, in upstream order. */

/**
 * Ok
 * @var int
 * @cvalue SAUDIO_LOGITEM_OK
 */
const SAUDIO_LOGITEM_OK = UNKNOWN;

/**
 * memory allocation failed
 * @var int
 * @cvalue SAUDIO_LOGITEM_MALLOC_FAILED
 */
const SAUDIO_LOGITEM_MALLOC_FAILED = UNKNOWN;

/**
 * snd_pcm_open() failed
 * @var int
 * @cvalue SAUDIO_LOGITEM_ALSA_SND_PCM_OPEN_FAILED
 */
const SAUDIO_LOGITEM_ALSA_SND_PCM_OPEN_FAILED = UNKNOWN;

/**
 * floating point sample format not supported
 * @var int
 * @cvalue SAUDIO_LOGITEM_ALSA_FLOAT_SAMPLES_NOT_SUPPORTED
 */
const SAUDIO_LOGITEM_ALSA_FLOAT_SAMPLES_NOT_SUPPORTED = UNKNOWN;

/**
 * requested buffer size not supported
 * @var int
 * @cvalue SAUDIO_LOGITEM_ALSA_REQUESTED_BUFFER_SIZE_NOT_SUPPORTED
 */
const SAUDIO_LOGITEM_ALSA_REQUESTED_BUFFER_SIZE_NOT_SUPPORTED = UNKNOWN;

/**
 * requested channel count not supported
 * @var int
 * @cvalue SAUDIO_LOGITEM_ALSA_REQUESTED_CHANNEL_COUNT_NOT_SUPPORTED
 */
const SAUDIO_LOGITEM_ALSA_REQUESTED_CHANNEL_COUNT_NOT_SUPPORTED = UNKNOWN;

/**
 * snd_pcm_hw_params_set_rate_near() failed
 * @var int
 * @cvalue SAUDIO_LOGITEM_ALSA_SND_PCM_HW_PARAMS_SET_RATE_NEAR_FAILED
 */
const SAUDIO_LOGITEM_ALSA_SND_PCM_HW_PARAMS_SET_RATE_NEAR_FAILED = UNKNOWN;

/**
 * snd_pcm_hw_params() failed
 * @var int
 * @cvalue SAUDIO_LOGITEM_ALSA_SND_PCM_HW_PARAMS_FAILED
 */
const SAUDIO_LOGITEM_ALSA_SND_PCM_HW_PARAMS_FAILED = UNKNOWN;

/**
 * pthread_create() failed
 * @var int
 * @cvalue SAUDIO_LOGITEM_ALSA_PTHREAD_CREATE_FAILED
 */
const SAUDIO_LOGITEM_ALSA_PTHREAD_CREATE_FAILED = UNKNOWN;

/**
 * requested sample rate or channel count not supported
 * @var int
 * @cvalue SAUDIO_LOGITEM_PULSEAUDIO_INVALID_SAMPLE_SPEC
 */
const SAUDIO_LOGITEM_PULSEAUDIO_INVALID_SAMPLE_SPEC = UNKNOWN;

/**
 * pa_simple_new() failed
 * @var int
 * @cvalue SAUDIO_LOGITEM_PULSEAUDIO_SIMPLE_NEW_FAILED
 */
const SAUDIO_LOGITEM_PULSEAUDIO_SIMPLE_NEW_FAILED = UNKNOWN;

/**
 * pa_simple_write() failed (connection to server lost)
 * @var int
 * @cvalue SAUDIO_LOGITEM_PULSEAUDIO_SIMPLE_WRITE_FAILED
 */
const SAUDIO_LOGITEM_PULSEAUDIO_SIMPLE_WRITE_FAILED = UNKNOWN;

/**
 * pthread_create() failed
 * @var int
 * @cvalue SAUDIO_LOGITEM_PULSEAUDIO_PTHREAD_CREATE_FAILED
 */
const SAUDIO_LOGITEM_PULSEAUDIO_PTHREAD_CREATE_FAILED = UNKNOWN;

/**
 * CreateEvent() failed
 * @var int
 * @cvalue SAUDIO_LOGITEM_WASAPI_CREATE_EVENT_FAILED
 */
const SAUDIO_LOGITEM_WASAPI_CREATE_EVENT_FAILED = UNKNOWN;

/**
 * CoCreateInstance() for IMMDeviceEnumerator failed
 * @var int
 * @cvalue SAUDIO_LOGITEM_WASAPI_CREATE_DEVICE_ENUMERATOR_FAILED
 */
const SAUDIO_LOGITEM_WASAPI_CREATE_DEVICE_ENUMERATOR_FAILED = UNKNOWN;

/**
 * IMMDeviceEnumerator.GetDefaultAudioEndpoint() failed
 * @var int
 * @cvalue SAUDIO_LOGITEM_WASAPI_GET_DEFAULT_AUDIO_ENDPOINT_FAILED
 */
const SAUDIO_LOGITEM_WASAPI_GET_DEFAULT_AUDIO_ENDPOINT_FAILED = UNKNOWN;

/**
 * IMMDevice.Activate() failed
 * @var int
 * @cvalue SAUDIO_LOGITEM_WASAPI_DEVICE_ACTIVATE_FAILED
 */
const SAUDIO_LOGITEM_WASAPI_DEVICE_ACTIVATE_FAILED = UNKNOWN;

/**
 * IAudioClient.Initialize() failed
 * @var int
 * @cvalue SAUDIO_LOGITEM_WASAPI_AUDIO_CLIENT_INITIALIZE_FAILED
 */
const SAUDIO_LOGITEM_WASAPI_AUDIO_CLIENT_INITIALIZE_FAILED = UNKNOWN;

/**
 * IAudioClient.GetBufferSize() failed
 * @var int
 * @cvalue SAUDIO_LOGITEM_WASAPI_AUDIO_CLIENT_GET_BUFFER_SIZE_FAILED
 */
const SAUDIO_LOGITEM_WASAPI_AUDIO_CLIENT_GET_BUFFER_SIZE_FAILED = UNKNOWN;

/**
 * IAudioClient.GetService() failed
 * @var int
 * @cvalue SAUDIO_LOGITEM_WASAPI_AUDIO_CLIENT_GET_SERVICE_FAILED
 */
const SAUDIO_LOGITEM_WASAPI_AUDIO_CLIENT_GET_SERVICE_FAILED = UNKNOWN;

/**
 * IAudioClient.SetEventHandle() failed
 * @var int
 * @cvalue SAUDIO_LOGITEM_WASAPI_AUDIO_CLIENT_SET_EVENT_HANDLE_FAILED
 */
const SAUDIO_LOGITEM_WASAPI_AUDIO_CLIENT_SET_EVENT_HANDLE_FAILED = UNKNOWN;

/**
 * CreateThread() failed
 * @var int
 * @cvalue SAUDIO_LOGITEM_WASAPI_CREATE_THREAD_FAILED
 */
const SAUDIO_LOGITEM_WASAPI_CREATE_THREAD_FAILED = UNKNOWN;

/**
 * AAudioStreamBuilder_openStream() failed
 * @var int
 * @cvalue SAUDIO_LOGITEM_AAUDIO_STREAMBUILDER_OPEN_STREAM_FAILED
 */
const SAUDIO_LOGITEM_AAUDIO_STREAMBUILDER_OPEN_STREAM_FAILED = UNKNOWN;

/**
 * pthread_create() failed after AAUDIO_ERROR_DISCONNECTED
 * @var int
 * @cvalue SAUDIO_LOGITEM_AAUDIO_PTHREAD_CREATE_FAILED
 */
const SAUDIO_LOGITEM_AAUDIO_PTHREAD_CREATE_FAILED = UNKNOWN;

/**
 * restarting AAudio stream after error
 * @var int
 * @cvalue SAUDIO_LOGITEM_AAUDIO_RESTARTING_STREAM_AFTER_ERROR
 */
const SAUDIO_LOGITEM_AAUDIO_RESTARTING_STREAM_AFTER_ERROR = UNKNOWN;

/**
 * using AAudio backend
 * @var int
 * @cvalue SAUDIO_LOGITEM_USING_AAUDIO_BACKEND
 */
const SAUDIO_LOGITEM_USING_AAUDIO_BACKEND = UNKNOWN;

/**
 * AAudio_createStreamBuilder() failed
 * @var int
 * @cvalue SAUDIO_LOGITEM_AAUDIO_CREATE_STREAMBUILDER_FAILED
 */
const SAUDIO_LOGITEM_AAUDIO_CREATE_STREAMBUILDER_FAILED = UNKNOWN;

/**
 * AudioQueueNewOutput() failed
 * @var int
 * @cvalue SAUDIO_LOGITEM_COREAUDIO_NEW_OUTPUT_FAILED
 */
const SAUDIO_LOGITEM_COREAUDIO_NEW_OUTPUT_FAILED = UNKNOWN;

/**
 * AudioQueueAllocateBuffer() failed
 * @var int
 * @cvalue SAUDIO_LOGITEM_COREAUDIO_ALLOCATE_BUFFER_FAILED
 */
const SAUDIO_LOGITEM_COREAUDIO_ALLOCATE_BUFFER_FAILED = UNKNOWN;

/**
 * AudioQueueStart() failed
 * @var int
 * @cvalue SAUDIO_LOGITEM_COREAUDIO_START_FAILED
 */
const SAUDIO_LOGITEM_COREAUDIO_START_FAILED = UNKNOWN;

/**
 * backend buffer size isn't multiple of packet size
 * @var int
 * @cvalue SAUDIO_LOGITEM_BACKEND_BUFFER_SIZE_ISNT_MULTIPLE_OF_PACKET_SIZE
 */
const SAUDIO_LOGITEM_BACKEND_BUFFER_SIZE_ISNT_MULTIPLE_OF_PACKET_SIZE = UNKNOWN;

/**
 * sceAudioOutOpenPort() failed
 * @var int
 * @cvalue SAUDIO_LOGITEM_VITA_SCEAUDIO_OPEN_FAILED
 */
const SAUDIO_LOGITEM_VITA_SCEAUDIO_OPEN_FAILED = UNKNOWN;

/**
 * pthread_create() failed
 * @var int
 * @cvalue SAUDIO_LOGITEM_VITA_PTHREAD_CREATE_FAILED
 */
const SAUDIO_LOGITEM_VITA_PTHREAD_CREATE_FAILED = UNKNOWN;

/**
 * ndspInit() failed
 * @var int
 * @cvalue SAUDIO_LOGITEM_N3DS_NDSP_OPEN_FAILED
 */
const SAUDIO_LOGITEM_N3DS_NDSP_OPEN_FAILED = UNKNOWN;

/* saudio_n3ds_ndspinterptype */

/**
 * @var int
 * @cvalue SAUDIO_N3DS_DSP_INTERP_POLYPHASE
 */
const SAUDIO_N3DS_DSP_INTERP_POLYPHASE = UNKNOWN;

/**
 * @var int
 * @cvalue SAUDIO_N3DS_DSP_INTERP_LINEAR
 */
const SAUDIO_N3DS_DSP_INTERP_LINEAR = UNKNOWN;

/**
 * @var int
 * @cvalue SAUDIO_N3DS_DSP_INTERP_NONE
 */
const SAUDIO_N3DS_DSP_INTERP_NONE = UNKNOWN;

/**
 * Custom logging and error reporting callback.
 * func: fn(string $tag, int $log_level, int $log_item_id, ?string $message_or_null, int $line_nr, ?string $filename_or_null, mixed $user_data): void
 */
final class saudio_logger
{
    public ?Closure $func = null;
    public mixed $user_data = null;

    public function __construct(?Closure $func = null, mixed $user_data = null) {}
}

/**
 * Memory-alloc and -free override. PHP has no memory to hand sokol-audio:
 * saudio_setup() accepts only null functions, and sokol-audio keeps malloc/free.
 */
final class saudio_allocator
{
    public ?Closure $alloc_fn = null;
    public ?Closure $free_fn = null;
    public mixed $user_data = null;

    public function __construct(?Closure $alloc_fn = null, ?Closure $free_fn = null, mixed $user_data = null) {}
}

final class saudio_n3ds_desc
{
    /** the 3DS requires multiple queues that it alternates between; default 2 */
    public int $queue_count = 0;
    /** SAUDIO_N3DS_DSP_INTERP_*; default 0 */
    public int $interpolation_type = 0;
    /** valid range 0-23; default 0 */
    public int $channel_id = 0;

    public function __construct(int $queue_count = 0, int $interpolation_type = 0, int $channel_id = 0) {}
}

final class saudio_win32_desc
{
    /** when true sokol-audio will not call CoInitializeEx/CoUninitialze */
    public bool $skip_coinitialize = false;

    public function __construct(bool $skip_coinitialize = false) {}
}

/**
 * stream_cb: fn(string &$buffer, int $num_frames, int $num_channels): void
 * stream_userdata_cb: fn(string &$buffer, int $num_frames, int $num_channels, mixed $user_data): void
 * $buffer holds num_frames * num_channels packed 32-bit floats (pack('f*', ...)).
 */
final class saudio_desc
{
    /** requested sample rate */
    public int $sample_rate = 0;
    /** number of channels, default: 1 (mono) */
    public int $num_channels = 0;
    /** number of frames in streaming buffer */
    public int $buffer_frames = 0;
    /** number of frames in a packet */
    public int $packet_frames = 0;
    /** number of packets in packet queue */
    public int $num_packets = 0;
    /** optional streaming callback (no user data) */
    public ?Closure $stream_cb = null;
    /** ... and with user data */
    public ?Closure $stream_userdata_cb = null;
    /** optional user data argument for stream_userdata_cb */
    public mixed $user_data = null;
    /** optional config options for windows */
    public saudio_win32_desc $win32;
    /** optional data for use on n3ds */
    public saudio_n3ds_desc $n3ds;
    /** optional allocation override functions */
    public saudio_allocator $allocator;
    /** optional logging function (default: NO LOGGING!) */
    public saudio_logger $logger;

    public function __construct(int $sample_rate = 0, int $num_channels = 0, int $buffer_frames = 0, int $packet_frames = 0, int $num_packets = 0, ?Closure $stream_cb = null, ?Closure $stream_userdata_cb = null, mixed $user_data = null, saudio_win32_desc $win32 = new saudio_win32_desc(), saudio_n3ds_desc $n3ds = new saudio_n3ds_desc(), saudio_allocator $allocator = new saudio_allocator(), saudio_logger $logger = new saudio_logger()) {}
}

/** A sokol-audio SOKOL_ASSERT that failed inside the library. */
final class saudio_assertion_error extends Error
{
}

/** setup sokol-audio */
function saudio_setup(saudio_desc $desc): void {}

/** shutdown sokol-audio */
function saudio_shutdown(): void {}

/** true after setup if audio backend was successfully initialized */
function saudio_isvalid(): bool {}

/** return the saudio_desc.user_data value */
function saudio_userdata(): mixed {}

/** return a copy of the original saudio_desc struct */
function saudio_query_desc(): saudio_desc {}

/** actual sample rate */
function saudio_sample_rate(): int {}

/** return actual backend buffer size in number of frames */
function saudio_buffer_frames(): int {}

/** actual number of channels */
function saudio_channels(): int {}

/** return true if audio context is currently suspended (only in WebAudio backend, all other backends return false) */
function saudio_suspended(): bool {}

/** get current number of frames to fill packet queue */
function saudio_expect(): int {}

/** push sample frames (packed 32-bit floats) from main thread, returns number of frames actually pushed */
function saudio_push(string $frames, int $num_frames): int {}
