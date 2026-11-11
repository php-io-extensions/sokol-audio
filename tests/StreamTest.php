<?php

declare(strict_types=1);

it('serves stream_cb with a zeroed buffer of num_frames x num_channels floats', function (): void {
    $seen = [];
    saudio_test_setup(new saudio_desc(num_channels: 2, buffer_frames: 1024, stream_cb: function (string &$buffer, int $frames, int $channels) use (&$seen): void {
        $seen[] = [strlen($buffer), $frames, $channels, trim($buffer, "\0")];
        $buffer = str_repeat(pack('f*', 0.0, 0.0), $frames);
    }));

    saudio_test_spin(0.5);

    expect(count($seen))->toBeGreaterThan(3);
    foreach ($seen as [$bytes, $frames, $channels, $nonZero]) {
        expect($channels)->toBe(2)
            ->and($bytes)->toBe($frames * 2 * 4)
            ->and($nonZero)->toBe('');
    }
});

it('hands stream_userdata_cb the desc user data', function (): void {
    $user = new stdClass();
    $got = [];
    saudio_test_setup(new saudio_desc(stream_userdata_cb: function (string &$buffer, int $frames, int $channels, mixed $userData) use (&$got): void {
        $got[] = $userData;
        $buffer = str_repeat("\0", $frames * $channels * 4);
    }, user_data: $user));

    saudio_test_spin(0.3);

    expect($got)->not->toBeEmpty();
    foreach ($got as $value) {
        expect($value)->toBe($user);
    }
});

it('calls stream_cb, not stream_userdata_cb, when both are set', function (): void {
    $plain = $withData = 0;
    saudio_test_setup(new saudio_desc(
        stream_cb: function (string &$buffer) use (&$plain): void { $plain++; },
        stream_userdata_cb: function (string &$buffer) use (&$withData): void { $withData++; },
    ));

    saudio_test_spin(0.3);

    expect($plain)->toBeGreaterThan(0)
        ->and($withData)->toBe(0);
});

it('lets the audio thread go on with silence while the script is not at a VM interrupt point', function (): void {
    $calls = 0;
    saudio_test_setup(new saudio_desc(buffer_frames: 512, stream_cb: function (string &$buffer) use (&$calls): void { $calls++; }));
    saudio_test_spin(0.1);

    $before = $calls;
    usleep(400000); /* about 34 buffers of 512 frames at 44.1 kHz */
    $after = $calls;

    expect($after - $before)->toBeLessThanOrEqual(1);
    saudio_test_spin(0.2);
    expect($calls)->toBeGreaterThan($after);
});

/*
 * The first request can be served at any interrupt point after saudio_setup() returns (PulseAudio asks at once,
 * CoreAudio after its two primed buffers), so setup sits inside the expectation.
 */
it('rethrows an exception from the stream callback at the interrupt point', function (): void {
    saudio_test_require_output();

    expect(function (): void {
        saudio_setup(new saudio_desc(stream_cb: function (): void {
            throw new RuntimeException('stream failed');
        }));
        saudio_test_spin(1.0);
    })->toThrow(RuntimeException::class, 'stream failed')
        ->and(saudio_isvalid())->toBeTrue();
});

it('throws when the callback leaves $buffer the wrong size or type', function (Closure $cb, string $class, string $message): void {
    saudio_test_require_output();

    expect(function () use ($cb): void {
        saudio_setup(new saudio_desc(num_channels: 2, buffer_frames: 1024, stream_cb: $cb));
        saudio_test_spin(1.0);
    })->toThrow($class, $message);
})->with([
    'short' => [function (string &$buffer): void { $buffer = 'abc'; }, ValueError::class, 'sokol-audio stream callback must leave $buffer 8192 bytes (1024 frames x 2 channels x 4), 3 given'],
    'not a string' => [function (&$buffer): void { $buffer = 1.5; }, TypeError::class, 'sokol-audio stream callback must leave $buffer a string, float given'],
]);

it('serves callbacks again after shutdown and a new setup', function (): void {
    $calls = 0;
    $desc = new saudio_desc(stream_cb: function () use (&$calls): void { $calls++; });
    saudio_test_setup($desc);
    saudio_test_spin(0.2);
    saudio_shutdown();
    $stopped = $calls;
    saudio_test_spin(0.2);
    expect($calls)->toBe($stopped);

    saudio_test_setup($desc);
    saudio_test_spin(0.2);
    expect($calls)->toBeGreaterThan($stopped);
});
