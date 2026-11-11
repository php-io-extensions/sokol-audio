PHP_ARG_ENABLE([sokol-audio],
  [whether to enable sokol_audio support],
  [AS_HELP_STRING([--enable-sokol-audio], [Enable the sokol_audio.h bindings])],
  [no])

if test "$PHP_SOKOL_AUDIO" != "no"; then
  dnl lib/sokol/sokol_audio.h is compiled once, by src/sokol_audio_impl.c.
  PHP_NEW_EXTENSION([sokol_audio],
    [src/sokol_audio.c src/callbacks.c src/sokol_audio_impl.c],
    [$ext_shared],,
    [-std=gnu17 -fvisibility=hidden -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1 -I$ext_srcdir/lib/sokol])
  PHP_ADD_BUILD_DIR([$ext_builddir/src])

  dnl Upstream's backend for the platform: CoreAudio on macOS, PulseAudio on Linux.
  case $host_os in
    darwin*)
      dnl Shared, the framework goes to the .so; compiled in, to PHP's own link line.
      if test "$ext_shared" = "yes"; then
        SOKOL_AUDIO_SHARED_LIBADD="$SOKOL_AUDIO_SHARED_LIBADD -framework AudioToolbox"
      else
        PHP_ADD_FRAMEWORK([AudioToolbox])
      fi
      ;;
    *)
      AC_CHECK_HEADER([pulse/simple.h], [],
        [AC_MSG_ERROR([pulse/simple.h not found; install the PulseAudio development headers (Debian: apt install libpulse-dev)])])
      PHP_CHECK_LIBRARY([pulse-simple], [pa_simple_new],
        [PHP_ADD_LIBRARY([pulse-simple],, [SOKOL_AUDIO_SHARED_LIBADD])
         PHP_ADD_LIBRARY([pulse],, [SOKOL_AUDIO_SHARED_LIBADD])],
        [AC_MSG_ERROR([libpulse-simple not found; install libpulse-dev])],
        [-lpulse])
      PHP_ADD_LIBRARY([pthread],, [SOKOL_AUDIO_SHARED_LIBADD])
      PHP_ADD_LIBRARY([m],, [SOKOL_AUDIO_SHARED_LIBADD])
      ;;
  esac
  PHP_SUBST([SOKOL_AUDIO_SHARED_LIBADD])
fi
