/*
 * The one translation unit that compiles sokol_audio.h's implementation,
 * configured only through the knobs upstream documents:
 *
 *   SOKOL_ASSERT  records the failure in the extension's slot instead of
 *                 calling assert(); the binding throws saudio_assertion_error
 *                 when the call returns.
 *   SOKOL_DEBUG   always on, so the logger gets upstream's message strings
 *                 and file name in every build, not only in builds without
 *                 NDEBUG.
 *
 * The backend is upstream's choice for the platform: CoreAudio on macOS,
 * PulseAudio on Linux.
 */

#include "runtime.h"

#define SOKOL_ASSERT(c) ((c) ? (void)0 : phpsaudio_on_assert(#c, __FILE__, __LINE__))
#ifndef SOKOL_DEBUG
#define SOKOL_DEBUG
#endif
#define SOKOL_AUDIO_IMPL
#include "sokol_audio.h"

/* Upstream values the binding's guards need, read from the header that defines them. */
const int phpsaudio_ring_max_slots = SAUDIO_RING_MAX_SLOTS;
const int phpsaudio_default_sample_rate = _SAUDIO_DEFAULT_SAMPLE_RATE;
const int phpsaudio_default_buffer_frames = _SAUDIO_DEFAULT_BUFFER_FRAMES;
const int phpsaudio_default_packet_frames = _SAUDIO_DEFAULT_PACKET_FRAMES;
const int phpsaudio_default_num_packets = _SAUDIO_DEFAULT_NUM_PACKETS;
