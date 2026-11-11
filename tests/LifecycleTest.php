<?php

declare(strict_types=1);

const SAUDIO_TEST_LIVE = <<<'PHP'
saudio_setup(new saudio_desc(stream_cb: function (string &$buffer): void {}, logger: new saudio_logger(fn () => null)));
if (! saudio_isvalid()) { echo 'no output'; exit(0); }
$until = microtime(true) + 0.2;
while (microtime(true) < $until) { usleep(1000); }
PHP;

it('ends a request that left sokol-audio running without crashing or reporting', function (): void {
    [$status, $stdout, $stderr] = saudio_test_subprocess(SAUDIO_TEST_LIVE.' echo "played";');

    if ($stdout === 'no output') {
        test()->markTestSkipped('no audio output on this machine');
    }
    expect($status)->toBe(0)
        ->and($stdout)->toBe('played')
        ->and($stderr)->toBe('');
});

it('refuses sokol-audio in a forked child and lets it exit cleanly', function (): void {
    if (! function_exists('pcntl_fork')) {
        test()->markTestSkipped('needs pcntl');
    }
    [$status, $stdout, $stderr] = saudio_test_subprocess(SAUDIO_TEST_LIVE.<<<'PHP'
        $pid = pcntl_fork();
        if ($pid === 0) {
            foreach (['saudio_push' => fn () => saudio_push(pack('f', 0.0), 1), 'saudio_setup' => fn () => saudio_setup(new saudio_desc()), 'saudio_shutdown' => fn () => saudio_shutdown()] as $name => $call) {
                try { $call(); echo "$name passed\n"; } catch (Error $e) { echo "$name: ", $e->getMessage(), "\n"; }
            }
            exit(0);
        }
        pcntl_waitpid($pid, $child);
        echo 'child ', pcntl_wexitstatus($child), "\n";
        $until = microtime(true) + 0.1;
        while (microtime(true) < $until) { usleep(1000); }
        saudio_shutdown();
        echo 'parent shut down';
        PHP);

    if ($stdout === 'no output') {
        test()->markTestSkipped('no audio output on this machine');
    }
    expect($status)->toBe(0)
        ->and($stdout)->toBe(
            "saudio_push: sokol-audio's audio thread did not survive fork\n"
            ."saudio_setup: sokol-audio's audio thread did not survive fork\n"
            ."saudio_shutdown: sokol-audio's audio thread did not survive fork\n"
            ."child 0\n"
            .'parent shut down'
        )
        ->and($stderr)->toBe('');
});
