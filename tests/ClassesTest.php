<?php

declare(strict_types=1);

it('binds every saudio_log_item in upstream order', function (): void {
    $items = ['OK', 'MALLOC_FAILED', 'ALSA_SND_PCM_OPEN_FAILED', 'ALSA_FLOAT_SAMPLES_NOT_SUPPORTED',
        'ALSA_REQUESTED_BUFFER_SIZE_NOT_SUPPORTED', 'ALSA_REQUESTED_CHANNEL_COUNT_NOT_SUPPORTED',
        'ALSA_SND_PCM_HW_PARAMS_SET_RATE_NEAR_FAILED', 'ALSA_SND_PCM_HW_PARAMS_FAILED', 'ALSA_PTHREAD_CREATE_FAILED',
        'PULSEAUDIO_INVALID_SAMPLE_SPEC', 'PULSEAUDIO_SIMPLE_NEW_FAILED', 'PULSEAUDIO_SIMPLE_WRITE_FAILED',
        'PULSEAUDIO_PTHREAD_CREATE_FAILED', 'WASAPI_CREATE_EVENT_FAILED', 'WASAPI_CREATE_DEVICE_ENUMERATOR_FAILED',
        'WASAPI_GET_DEFAULT_AUDIO_ENDPOINT_FAILED', 'WASAPI_DEVICE_ACTIVATE_FAILED', 'WASAPI_AUDIO_CLIENT_INITIALIZE_FAILED',
        'WASAPI_AUDIO_CLIENT_GET_BUFFER_SIZE_FAILED', 'WASAPI_AUDIO_CLIENT_GET_SERVICE_FAILED',
        'WASAPI_AUDIO_CLIENT_SET_EVENT_HANDLE_FAILED', 'WASAPI_CREATE_THREAD_FAILED', 'AAUDIO_STREAMBUILDER_OPEN_STREAM_FAILED',
        'AAUDIO_PTHREAD_CREATE_FAILED', 'AAUDIO_RESTARTING_STREAM_AFTER_ERROR', 'USING_AAUDIO_BACKEND',
        'AAUDIO_CREATE_STREAMBUILDER_FAILED', 'COREAUDIO_NEW_OUTPUT_FAILED', 'COREAUDIO_ALLOCATE_BUFFER_FAILED',
        'COREAUDIO_START_FAILED', 'BACKEND_BUFFER_SIZE_ISNT_MULTIPLE_OF_PACKET_SIZE', 'VITA_SCEAUDIO_OPEN_FAILED',
        'VITA_PTHREAD_CREATE_FAILED', 'N3DS_NDSP_OPEN_FAILED'];

    foreach ($items as $value => $item) {
        expect(constant("SAUDIO_LOGITEM_{$item}"))->toBe($value);
    }
    expect(array_filter(array_keys(get_defined_constants(true)['sokol_audio']), fn ($n) => str_starts_with($n, 'SAUDIO_LOGITEM_')))
        ->toHaveCount(count($items));
});

it('binds saudio_n3ds_ndspinterptype', function (): void {
    expect(SAUDIO_N3DS_DSP_INTERP_POLYPHASE)->toBe(0)
        ->and(SAUDIO_N3DS_DSP_INTERP_LINEAR)->toBe(1)
        ->and(SAUDIO_N3DS_DSP_INTERP_NONE)->toBe(2);
});

it('zero-initialises saudio_desc like the C aggregate initializer', function (): void {
    $desc = new saudio_desc();

    expect($desc->sample_rate)->toBe(0)
        ->and($desc->num_channels)->toBe(0)
        ->and($desc->buffer_frames)->toBe(0)
        ->and($desc->packet_frames)->toBe(0)
        ->and($desc->num_packets)->toBe(0)
        ->and($desc->stream_cb)->toBeNull()
        ->and($desc->stream_userdata_cb)->toBeNull()
        ->and($desc->user_data)->toBeNull()
        ->and($desc->win32)->toEqual(new saudio_win32_desc(false))
        ->and($desc->n3ds)->toEqual(new saudio_n3ds_desc(0, 0, 0))
        ->and($desc->allocator)->toEqual(new saudio_allocator(null, null, null))
        ->and($desc->logger)->toEqual(new saudio_logger(null, null));
});

it('takes every field positionally and by name, in field order', function (): void {
    $cb = fn (string &$buffer, int $frames, int $channels) => null;
    $logger = new saudio_logger(fn () => null, 'log');
    $desc = new saudio_desc(48000, 2, 1024, 256, 32, $cb, null, ['ctx'], new saudio_win32_desc(true), new saudio_n3ds_desc(3, SAUDIO_N3DS_DSP_INTERP_LINEAR, 5), new saudio_allocator(user_data: 7), $logger);

    expect([$desc->sample_rate, $desc->num_channels, $desc->buffer_frames, $desc->packet_frames, $desc->num_packets])->toBe([48000, 2, 1024, 256, 32])
        ->and($desc->stream_cb)->toBe($cb)
        ->and($desc->user_data)->toBe(['ctx'])
        ->and($desc->win32->skip_coinitialize)->toBeTrue()
        ->and([$desc->n3ds->queue_count, $desc->n3ds->interpolation_type, $desc->n3ds->channel_id])->toBe([3, 1, 5])
        ->and($desc->allocator->user_data)->toBe(7)
        ->and($desc->logger)->toBe($logger);

    $named = new saudio_desc(num_packets: 8, logger: $logger);
    expect($named->num_packets)->toBe(8)
        ->and($named->sample_rate)->toBe(0)
        ->and($named->win32)->toBeInstanceOf(saudio_win32_desc::class)
        ->and($named->logger)->toBe($logger);
});

it('gives each saudio_desc its own nested structs', function (): void {
    $a = new saudio_desc();
    $b = new saudio_desc();

    expect($a->logger)->not->toBe($b->logger)
        ->and($a->n3ds)->not->toBe($b->n3ds);
});

it('copies nested structs on clone, and shares closures and user data', function (): void {
    $user = new stdClass();
    $desc = new saudio_desc(user_data: $user, logger: new saudio_logger(fn () => null, $user));
    $copy = clone $desc;
    $copy->logger->user_data = 'changed';
    $copy->n3ds->channel_id = 9;

    expect($desc->logger->user_data)->toBe($user)
        ->and($desc->n3ds->channel_id)->toBe(0)
        ->and($copy->user_data)->toBe($user)
        ->and($copy->logger->func)->toBe($desc->logger->func);
});

it('rejects more arguments than fields and wrongly typed fields', function (): void {
    expect(fn () => new saudio_win32_desc(true, false))->toThrow(ArgumentCountError::class)
        ->and(fn () => new saudio_n3ds_desc('three'))->toThrow(TypeError::class);
});

it('declares saudio_assertion_error as an Error', function (): void {
    expect(is_subclass_of(saudio_assertion_error::class, Error::class))->toBeTrue();
});
