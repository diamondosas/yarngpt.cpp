#ifndef YARNGPT_H
#define YARNGPT_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Opaque handle for the Yarngpt context
typedef struct yarngpt_context yarngpt_context_t;

// Configuration for initializing the Yarngpt engine
typedef struct {
    const char* model_path;      // Path to the GGUF language model
    const char* wav_model_path;  // Path to the WavTokenizer TorchScript model
    int n_threads;               // Number of threads for inference
    int n_ctx;                   // Context size for the LLM
} yarngpt_config_t;

// Callback function type for audio streaming
// Will be called when new audio chunk is generated
// audio_data: pointer to PCM float samples (24kHz, Mono)
// num_samples: number of samples in the chunk
// user_data: arbitrary user data passed to the callback
typedef void (*yarngpt_audio_callback)(const float* audio_data, int num_samples, void* user_data);

// Initialize the Yarngpt context
yarngpt_context_t* yarngpt_init(yarngpt_config_t config);

// Free the Yarngpt context
void yarngpt_free(yarngpt_context_t* ctx);

// Generate audio in a streaming fashion
// text: The input text to synthesize
// speaker: The speaker preset name (e.g. "idera", "jude")
// temp: Temperature for sampling (e.g. 0.1)
// cb: Callback function to receive audio chunks
// user_data: Pointer to pass to the callback
// Returns 0 on success, non-zero on failure.
int yarngpt_generate_stream(yarngpt_context_t* ctx, const char* text, const char* speaker, float temp, yarngpt_audio_callback cb, void* user_data);

#ifdef __cplusplus
}
#endif

#endif // YARNGPT_H
