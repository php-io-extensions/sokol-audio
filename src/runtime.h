#ifndef PHPSAUDIO_RUNTIME_H
#define PHPSAUDIO_RUNTIME_H

/* Shared with src/sokol_audio_impl.c, which compiles upstream: plain C, no php.h. */

#include <stdint.h>

/* SOKOL_ASSERT handler. Records the first failure; the binding throws after the call. */
void phpsaudio_on_assert(const char *condition, const char *file, int line);

/* C callbacks handed to sokol-audio in place of the PHP closures (src/callbacks.c). */
void phpsaudio_stream_cb(float *buffer, int num_frames, int num_channels);
void phpsaudio_stream_userdata_cb(float *buffer, int num_frames, int num_channels, void *user_data);
void phpsaudio_logger(const char *tag, uint32_t log_level, uint32_t log_item_id, const char *message_or_null,
	uint32_t line_nr, const char *filename_or_null, void *user_data);

extern const int phpsaudio_ring_max_slots;
extern const int phpsaudio_default_sample_rate;
extern const int phpsaudio_default_buffer_frames;
extern const int phpsaudio_default_packet_frames;
extern const int phpsaudio_default_num_packets;

#endif
