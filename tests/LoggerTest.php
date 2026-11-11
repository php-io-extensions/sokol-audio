<?php

declare(strict_types=1);

it('calls the logger during setup with upstream\'s arguments', function (): void {
    saudio_test_require_output();
    $logs = [];
    saudio_setup(new saudio_desc(buffer_frames: 1000, packet_frames: 128, logger: new saudio_logger(function (...$args) use (&$logs): void {
        $logs[] = $args;
    }, ['user'])));

    expect(saudio_isvalid())->toBeFalse()
        ->and($logs)->toHaveCount(1);
    [$tag, $level, $item, $message, $line, $file, $user] = $logs[0];
    expect($tag)->toBe('saudio')
        ->and($level)->toBe(1)
        ->and($item)->toBe(SAUDIO_LOGITEM_BACKEND_BUFFER_SIZE_ISNT_MULTIPLE_OF_PACKET_SIZE)
        ->and($message)->toBe("BACKEND_BUFFER_SIZE_ISNT_MULTIPLE_OF_PACKET_SIZE: backend buffer size isn't multiple of packet size")
        ->and($line)->toBe(2521)
        ->and($file)->toEndWith('sokol_audio.h')
        ->and($user)->toBe(['user']);
});

it('rethrows a logger exception when saudio_setup() returns, with sokol-audio set up', function (): void {
    saudio_test_require_output();

    expect(fn () => saudio_setup(new saudio_desc(buffer_frames: 1000, packet_frames: 128, logger: new saudio_logger(function (): void {
        throw new LogicException('logger failed');
    }))))->toThrow(LogicException::class, 'logger failed')
        ->and(saudio_channels())->toBe(1);
});

/*
 * The audio thread logs only PULSEAUDIO_SIMPLE_WRITE_FAILED (Linux) and AAudio restarts (Android).
 * Run it alone with SAUDIO_TEST_KILL_PULSE=1: it restarts or kills the pulse server (CI's last step).
 */
it('delivers an audio thread log at the next interrupt point', function (): void {
    if (PHP_OS_FAMILY !== 'Linux' || getenv('SAUDIO_TEST_KILL_PULSE') !== '1') {
        test()->markTestSkipped('needs Linux and SAUDIO_TEST_KILL_PULSE=1: it kills or restarts the pulse server');
    }
    $logs = [];
    saudio_test_setup(new saudio_desc(logger: new saudio_logger(function (...$args) use (&$logs): void {
        $logs[] = $args;
    })));

    /*
     * Drop the client connection by killing every pipewire-pulse (else PulseAudio). Not systemctl restart: where
     * systemd has the unit but the server was started by hand (GitHub's runner), that starts a second server and
     * leaves sokol's connection up. Under systemd, socket activation brings the server back.
     */
    exec('pkill -x pipewire-pulse || pulseaudio --kill');
    saudio_test_spin(2.0);

    expect(array_column($logs, 2))->toContain(SAUDIO_LOGITEM_PULSEAUDIO_SIMPLE_WRITE_FAILED);
});
