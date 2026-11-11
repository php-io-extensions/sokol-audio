# Log

## 2026-11-11

* **StreamTest**: the exception and bad-buffer tests set up inside the expectation. PulseAudio requests the first buffer as soon as `saudio_setup()` returns (CoreAudio waits out two primed buffers), so the callback could be served, and throw, before the old `expect(fn () => spin)`. **LoggerTest**: the audio-thread log test restarts `pipewire-pulse` where `pulseaudio` is absent.

* Bundle created with ext-sokol-audio 0.11.0: sokol_audio.h vendored at floooh/sokol `93aa187b5d387cc24b7290b3ca3f13f61e139e7c`; 11 functions, 5 struct classes + `saudio_assertion_error`, 37 constants. Stream callback and logger served on the owner thread through the VM interrupt queue, silence after one buffer's duration. Suite 51 passed, 1 skipped (Linux-only PulseAudio kill test) on Homebrew PHP 8.4 NTS and ZTS. Pi 5 (Debian trixie, PHP 8.4.27 ZTS, PipeWire pulse server, HDMI speaker): installer builds clean, 51 passed + the audio-thread log test, 3 runs; README tone examples audible. [bindings](api/bindings.md), [guards](architecture/guards.md), [callbacks](architecture/callbacks.md), [build](runbooks/build.md).
