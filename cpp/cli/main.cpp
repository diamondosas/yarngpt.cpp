#include "../yarngpt.h"
#include <iostream>
#include <fstream>
#include <vector>

void audio_cb(const float* audio_data, int num_samples, void* user_data) {
    std::vector<float>* full_audio = static_cast<std::vector<float>*>(user_data);
    full_audio->insert(full_audio->end(), audio_data, audio_data + num_samples);
    std::cout << "." << std::flush; // progress indicator
}

int main(int argc, char** argv) {
    if (argc < 5) {
        std::cerr << "Usage: " << argv[0] << " <gguf_path> <wav_pt_path> <speaker> <text>" << std::endl;
        return 1;
    }

    yarngpt_config_t config;
    config.model_path = argv[1];
    config.wav_model_path = argv[2];
    config.n_threads = 4;
    config.n_ctx = 2048;

    std::cout << "Initializing YarnGPT engine..." << std::endl;
    yarngpt_context_t* ctx = yarngpt_init(config);
    if (!ctx) {
        std::cerr << "Failed to initialize engine." << std::endl;
        return 1;
    }

    std::vector<float> full_audio;

    std::cout << "Generating audio for text: '" << argv[4] << "' with speaker: " << argv[3] << std::endl;
    int res = yarngpt_generate_stream(ctx, argv[4], argv[3], 0.1f, audio_cb, &full_audio);
    std::cout << std::endl;

    if (res != 0) {
        std::cerr << "Generation failed." << std::endl;
    } else {
        std::cout << "Generated " << full_audio.size() << " samples of audio." << std::endl;
        // Output to raw PCM file
        std::ofstream out("output.raw", std::ios::binary);
        out.write(reinterpret_cast<const char*>(full_audio.data()), full_audio.size() * sizeof(float));
        std::cout << "Saved to output.raw (24kHz, 32-bit float, mono)" << std::endl;
    }

    yarngpt_free(ctx);
    return 0;
}
