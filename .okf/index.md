---
okf_version: "0.2"
---

# ext-sokol-audio

1:1 PHP bindings of sokol_audio.h at floooh/sokol `93aa187b5d387cc24b7290b3ca3f13f61e139e7c` (vendored, unchanged). Version 0.11.0. Linux (PulseAudio), macOS (CoreAudio). PHP 8.4 NTS + ZTS.

# API

* [Bindings](api/bindings.md) — 11 functions, 5 struct classes + `saudio_assertion_error`, 37 constants; translations; what is not bound.

# Architecture

* [Guards and errors](architecture/guards.md) — Each guard and the SOKOL_ASSERT it stands in front of; assertion handler.
* [Callbacks and threads](architecture/callbacks.md) — Ownership, stream request queue, deadline, logs, shutdown, fork, request end.

# Runbooks

* [Build, install, test](runbooks/build.md) — Mac NTS/ZTS, scratch builds, Linux/Pi, CI null sink.
