<?php

declare(strict_types=1);

it('reports not valid before setup', function (): void {
    expect(saudio_isvalid())->toBeFalse();
});

it('rejects every stateful call before saudio_setup()', function (callable $call): void {
    expect($call)->toThrow(Error::class, 'saudio_setup() has not been called');
})->with([
    'saudio_shutdown' => [fn () => saudio_shutdown()],
    'saudio_userdata' => [fn () => saudio_userdata()],
    'saudio_query_desc' => [fn () => saudio_query_desc()],
    'saudio_sample_rate' => [fn () => saudio_sample_rate()],
    'saudio_buffer_frames' => [fn () => saudio_buffer_frames()],
    'saudio_channels' => [fn () => saudio_channels()],
    'saudio_suspended' => [fn () => saudio_suspended()],
    'saudio_expect' => [fn () => saudio_expect()],
    'saudio_push' => [fn () => saudio_push(pack('f', 0.0), 1)],
]);

it('rejects a second saudio_setup() until saudio_shutdown()', function (): void {
    saudio_setup(new saudio_desc());

    expect(fn () => saudio_setup(new saudio_desc()))->toThrow(Error::class, 'saudio_setup() was already called; call saudio_shutdown() first');

    saudio_shutdown();
    saudio_setup(new saudio_desc());
    expect(saudio_channels())->toBe(1);
});

it('rejects allocator functions and leaves sokol-audio unset', function (string $field): void {
    $desc = new saudio_desc(allocator: new saudio_allocator(...[$field => fn () => null]));

    expect(fn () => saudio_setup($desc))->toThrow(ValueError::class, "saudio_allocator::\${$field} must be null")
        ->and(fn () => saudio_channels())->toThrow(Error::class, 'saudio_setup() has not been called');
})->with(['alloc_fn', 'free_fn']);

it('rejects sizes sokol-audio would assert on or overflow', function (saudio_desc $desc, string $message): void {
    expect(fn () => saudio_setup($desc))->toThrow(ValueError::class, $message)
        ->and(saudio_isvalid())->toBeFalse();
})->with([
    'negative sample_rate' => [new saudio_desc(sample_rate: -1), 'saudio_desc::$sample_rate must be between 0 and'],
    'negative num_channels' => [new saudio_desc(num_channels: -2), 'saudio_desc::$num_channels must be between 0 and'],
    'negative buffer_frames' => [new saudio_desc(buffer_frames: -1), 'saudio_desc::$buffer_frames must be between 0 and'],
    'negative packet_frames' => [new saudio_desc(packet_frames: -1), 'saudio_desc::$packet_frames must be between 0 and'],
    'negative num_packets' => [new saudio_desc(num_packets: -1), 'saudio_desc::$num_packets must be between 0 and'],
    'over int' => [new saudio_desc(sample_rate: 2 ** 31), 'saudio_desc::$sample_rate must be between 0 and 2147483647'],
    'ring slots' => [new saudio_desc(num_packets: 1024), 'saudio_desc::$num_packets must be at most 1023 (SAUDIO_RING_MAX_SLOTS - 1), 1024 given'],
    'buffer bytes' => [new saudio_desc(num_channels: 2, buffer_frames: 2 ** 28), 'saudio_desc::$buffer_frames x $num_channels x 4 must be at most'],
    'fifo bytes' => [new saudio_desc(packet_frames: 2 ** 20, num_packets: 1000), 'saudio_desc::$packet_frames x $num_channels x 4 x $num_packets must be at most'],
]);

it('rejects an unset nested struct', function (): void {
    $desc = new saudio_desc();
    unset($desc->logger);

    expect(fn () => saudio_setup($desc))->toThrow(Error::class, 'must not be accessed before initialization');
});

it('rejects saudio_push() counts sokol-audio asserts on and short buffers', function (): void {
    saudio_setup(new saudio_desc(num_channels: 2));

    expect(fn () => saudio_push('', 0))->toThrow(ValueError::class, 'saudio_push(): Argument #2 ($num_frames) must be between 1 and 2147483647')
        ->and(fn () => saudio_push('', -4))->toThrow(ValueError::class, 'Argument #2 ($num_frames) must be between 1')
        ->and(fn () => saudio_push(pack('f*', 0.0, 0.0, 0.0), 2))->toThrow(ValueError::class, 'saudio_push(): Argument #1 ($frames) must hold 16 bytes (2 frames x 2 channels x 4), 12 given');
});

it('rejects saudio_shutdown() from inside a callback', function (): void {
    $caught = null;
    $desc = new saudio_desc(buffer_frames: 1000, packet_frames: 128, logger: new saudio_logger(function () use (&$caught): void {
        try {
            saudio_shutdown();
        } catch (Error $e) {
            $caught = $e->getMessage();
        }
    }));

    saudio_setup($desc);

    expect($caught)->toBe('saudio_shutdown() cannot be called from inside a sokol-audio callback');
});
