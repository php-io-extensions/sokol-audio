<?php

declare(strict_types=1);

it('reports the defaults sokol-audio picks for a zeroed desc', function (): void {
    saudio_test_setup(new saudio_desc());

    expect(saudio_sample_rate())->toBe(44100)
        ->and(saudio_channels())->toBe(1)
        ->and(saudio_buffer_frames())->toBe(2048)
        ->and(saudio_suspended())->toBeFalse()
        ->and(saudio_userdata())->toBeNull();
});

it('queues pushed frames until the packet queue is full', function (): void {
    saudio_test_setup(new saudio_desc(num_channels: 2, packet_frames: 128, num_packets: 16));

    expect(saudio_expect())->toBe(128 * 16);

    $frames = str_repeat(pack('f*', 0.0, 0.0), 128 * 16);
    expect(saudio_push($frames, 128 * 16))->toBe(128 * 16)
        ->and(saudio_push($frames, 128 * 16))->toBeLessThan(128 * 16)
        ->and(saudio_expect())->toBeLessThan(128 * 16);
});

it('reads only the first num_frames of a longer string', function (): void {
    saudio_test_setup(new saudio_desc());

    expect(saudio_push(pack('f*', 0.1, 0.2, 0.3), 2))->toBe(2);
});

it('pushes nothing when the backend did not come up', function (): void {
    saudio_test_require_output();
    /* 1000 is not a multiple of 128: sokol-audio logs BACKEND_BUFFER_SIZE_ISNT_MULTIPLE_OF_PACKET_SIZE and stays invalid. */
    saudio_setup(new saudio_desc(buffer_frames: 1000, packet_frames: 128));

    expect(saudio_isvalid())->toBeFalse()
        ->and(saudio_expect())->toBe(0)
        ->and(saudio_push(pack('f', 0.0), 1))->toBe(0);
});

it('returns a copy of the desc it was set up with', function (): void {
    $stream = fn (string &$buffer, int $frames, int $channels) => null;
    $log = fn () => null;
    $user = new stdClass();
    saudio_setup(new saudio_desc(
        sample_rate: 48000,
        num_channels: 2,
        buffer_frames: 1024,
        packet_frames: 256,
        num_packets: 8,
        stream_cb: $stream,
        user_data: $user,
        win32: new saudio_win32_desc(true),
        n3ds: new saudio_n3ds_desc(2, SAUDIO_N3DS_DSP_INTERP_NONE, 23),
        allocator: new saudio_allocator(user_data: 'heap'),
        logger: new saudio_logger($log, 'log'),
    ));

    $desc = saudio_query_desc();
    expect([$desc->sample_rate, $desc->num_channels, $desc->buffer_frames, $desc->packet_frames, $desc->num_packets])->toBe([48000, 2, 1024, 256, 8])
        ->and($desc->stream_cb)->toBe($stream)
        ->and($desc->stream_userdata_cb)->toBeNull()
        ->and($desc->user_data)->toBe($user)
        ->and(saudio_userdata())->toBe($user)
        ->and($desc->win32->skip_coinitialize)->toBeTrue()
        ->and([$desc->n3ds->queue_count, $desc->n3ds->interpolation_type, $desc->n3ds->channel_id])->toBe([2, 2, 23])
        ->and($desc->allocator)->toEqual(new saudio_allocator(null, null, 'heap'))
        ->and($desc->logger->func)->toBe($log)
        ->and($desc->logger->user_data)->toBe('log')
        ->and(saudio_query_desc())->not->toBe($desc);
});
