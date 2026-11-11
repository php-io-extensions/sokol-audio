# Support

- **Questions and bugs:** open an issue on [php-io-extensions/sokol-audio](https://github.com/php-io-extensions/sokol-audio/issues). Include `php --ri sokol_audio`, your OS and audio server, PHP NTS or ZTS, and the smallest script that shows the problem.
- **Security problems:** see [SECURITY.md](SECURITY.md); don't open a public issue.
- **sokol_audio.h itself:** the header's documentation is the manual; defects in it belong with [floooh/sokol](https://github.com/floooh/sokol).
- **No sound, `saudio_isvalid()` false:** set a `saudio_logger`; sokol reports why only through it. On Linux, check a PulseAudio or PipeWire pulse server is running for your user (`pactl info`).
