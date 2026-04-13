#include "yarngpt.h"
#include "llama_generator.h"
#include "wav_decoder.h"
#include <iostream>
#include <vector>

struct yarngpt_context {
    LlamaGenerator* llm;
    WavDecoder* decoder;
};

yarngpt_context_t* yarngpt_init(yarngpt_config_t config) {
    yarngpt_context_t* ctx = new yarngpt_context();

    ctx->llm = new LlamaGenerator(config.model_path, config.n_ctx, config.n_threads);
    if (!ctx->llm->load()) {
        delete ctx->llm;
        delete ctx;
        return nullptr;
    }

    ctx->decoder = new WavDecoder(config.wav_model_path);
    if (!ctx->decoder->load()) {
        delete ctx->llm;
        delete ctx->decoder;
        delete ctx;
        return nullptr;
    }

    return ctx;
}

void yarngpt_free(yarngpt_context_t* ctx) {
    if (ctx) {
        delete ctx->llm;
        delete ctx->decoder;
        delete ctx;
    }
}

int yarngpt_generate_stream(yarngpt_context_t* ctx, const char* text, const char* speaker, float temp, yarngpt_audio_callback cb, void* user_data) {
    if (!ctx || !ctx->llm || !ctx->decoder) return -1;

    std::string prompt = std::string("Speaker: ") + speaker + "\nText: " + text + "\n";
    std::vector<int> current_chunk;
    const int CHUNK_SIZE = 10; // Number of tokens to accumulate before decoding a chunk

    bool success = ctx->llm->generate(prompt, temp, [&](int token) {
        current_chunk.push_back(token);

        if (current_chunk.size() >= CHUNK_SIZE) {
            std::vector<float> audio = ctx->decoder->decode(current_chunk);
            if (cb && !audio.empty()) {
                cb(audio.data(), audio.size(), user_data);
            }
            current_chunk.clear();
        }
    });

    // Decode remaining
    if (!current_chunk.empty()) {
        std::vector<float> audio = ctx->decoder->decode(current_chunk);
        if (cb && !audio.empty()) {
            cb(audio.data(), audio.size(), user_data);
        }
    }

    return success ? 0 : -1;
}
