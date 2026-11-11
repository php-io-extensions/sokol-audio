# Security Policy

## Supported versions

ext-sokol-audio is pre-1.0. No 0.x release receives security fixes or advisories; fixes land in
the next release line. Security support starts with 1.0.

| Version | Security fixes |
|---------|----------------|
| < 1.0   | No             |

## Reporting a vulnerability

Please don't open a public issue for a security problem.

Report it privately through GitHub: the **Report a vulnerability** button on the
[php-io-extensions/sokol-audio](https://github.com/php-io-extensions/sokol-audio) repository's
**Security** tab. If that isn't available, email **info@projectsaturnstudios.com**.

Include what you found, the affected version, your platform and audio server (CoreAudio,
PulseAudio, PipeWire), PHP build (NTS or ZTS), the `saudio_desc` you set up with, and steps to
reproduce (`php --ri sokol_audio` shows the extension version and the sokol commit it carries).
Reports are read and weighed for the release line in development; before 1.0 there is no
response-time commitment.

A defect in sokol_audio.h itself belongs with [floooh/sokol](https://github.com/floooh/sokol).
Report it here as well when ext-sokol-audio should guard against it.

## Security model

ext-sokol-audio opens the platform's audio output in the PHP process and runs sokol's audio
thread there. It reads no files and opens no sockets of its own; on Linux, libpulse talks to the
PulseAudio server over its local socket.

- **Inputs.** Sizes are range-checked before sokol sees them: no negative sizes, at most
  `SAUDIO_RING_MAX_SLOTS - 1` packets, no byte count past `INT_MAX`. `saudio_push()` refuses a
  frame count below 1 and a string shorter than the frames it names. Allocator functions are
  refused.
- **State.** Calls before setup, a second setup, calls from another PHP thread, and setup or
  shutdown from inside a callback are `Error`s, so sokol's state asserts are never reached.
- **Assertions.** sokol's `SOKOL_ASSERT`s stay on in every build. The handler records the first
  failure and returns, so sokol never aborts; the binding throws `saudio_assertion_error`.
- **Audio thread.** It never enters PHP. It hands the stream buffer to the PHP thread that owns
  sokol-audio and waits; a request not started within one buffer's duration is withdrawn and
  filled with silence, so PHP never writes a buffer the audio thread has given up on. The PHP
  side copies exactly `num_frames * num_channels * 4` bytes back, and only from a string of that
  length.
- **Lifetime.** Request end shuts sokol-audio down and releases the held closures. After `fork()`
  the child has no audio thread; every stateful call there is an `Error`.

A report is in scope when ext-sokol-audio itself reads or writes memory it should not, leaks,
races, deadlocks or crashes: a buffer copied past its size, the audio thread touching PHP, a
closure called after release, a deadlock between shutdown and the audio thread, an audio thread
left after request end.
