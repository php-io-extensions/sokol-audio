<?php

declare(strict_types=1);

it('prints the extension version, the sokol commit and the backend', function (): void {
    ob_start();
    phpinfo(INFO_MODULES);
    $info = ob_get_clean();

    expect(phpversion('sokol_audio'))->toBe('0.11.0')
        ->and($info)->toContain('93aa187b5d387cc24b7290b3ca3f13f61e139e7c')
        ->and($info)->toContain(PHP_OS_FAMILY === 'Darwin' ? 'CoreAudio' : 'PulseAudio');
});
