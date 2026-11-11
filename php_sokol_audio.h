#ifndef PHP_SOKOL_AUDIO_H
#define PHP_SOKOL_AUDIO_H

extern zend_module_entry sokol_audio_module_entry;
#define phpext_sokol_audio_ptr &sokol_audio_module_entry

#define PHP_SOKOL_AUDIO_VERSION "0.11.0"

/* floooh/sokol has no release tags. A newer sokol_audio.h lands only by replacing lib/sokol/ from a later master commit. */
#define PHP_SOKOL_AUDIO_UPSTREAM_COMMIT "93aa187b5d387cc24b7290b3ca3f13f61e139e7c"

#if defined(ZTS) && defined(COMPILE_DL_SOKOL_AUDIO)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

#endif
