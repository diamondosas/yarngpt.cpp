#include "yarngpt.h"

extern void goAudioCallback(float* audio_data, int num_samples, void* user_data);

void cgo_audio_cb_wrapper(const float* audio_data, int num_samples, void* user_data) {
    goAudioCallback((float*)audio_data, num_samples, user_data);
}
