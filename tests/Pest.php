<?php

declare(strict_types=1);

if (! extension_loaded('sokol_audio')) {
    throw new RuntimeException('The sokol_audio extension is not loaded; run ./install-macos.sh or ./install-debian-trixie.sh');
}

/* Every test starts and ends without a live sokol-audio. */
uses()->afterEach(function (): void {
    try {
        saudio_shutdown();
    } catch (Error) {
    }
})->in(__DIR__);

/** Sets sokol-audio up and skips the test when the machine has no audio output (no device, no PulseAudio server). */
function saudio_test_setup(saudio_desc $desc): void
{
    saudio_setup($desc);
    if (! saudio_isvalid()) {
        test()->markTestSkipped('no audio output on this machine: saudio_isvalid() is false');
    }
}

/** Runs PHP code for $seconds so VM interrupts can serve sokol-audio's callbacks. */
function saudio_test_spin(float $seconds): void
{
    $until = microtime(true) + $seconds;
    while (microtime(true) < $until) {
        usleep(1000);
    }
}

/** Runs $code in a fresh PHP process with this process's ini; returns [exit code, stdout, stderr]. */
function saudio_test_subprocess(string $code): array
{
    $process = proc_open([PHP_BINARY, '-d', 'memory_limit=128M', '-r', $code], [1 => ['pipe', 'w'], 2 => ['pipe', 'w']], $pipes);
    $stdout = stream_get_contents($pipes[1]);
    $stderr = stream_get_contents($pipes[2]);
    fclose($pipes[1]);
    fclose($pipes[2]);

    return [proc_close($process), $stdout, $stderr];
}

/** Skips the test when sokol-audio cannot open the default output here. */
function saudio_test_require_output(): void
{
    saudio_setup(new saudio_desc());
    $valid = saudio_isvalid();
    saudio_shutdown();
    if (! $valid) {
        test()->markTestSkipped('no audio output on this machine: saudio_isvalid() is false');
    }
}
