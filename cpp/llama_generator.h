#ifndef LLAMA_GENERATOR_H
#define LLAMA_GENERATOR_H

#include <string>
#include <vector>
#include <functional>

// Forward declaration for llama_context, llama_model
struct llama_model;
struct llama_context;

class LlamaGenerator {
public:
    LlamaGenerator(const std::string& model_path, int n_ctx, int n_threads);
    ~LlamaGenerator();

    bool load();

    // Generates tokens and calls the callback for each generated integer code.
    // The model outputs discrete tokens (e.g. <|123|>) which we parse into integers.
    // Returns true on success.
    bool generate(const std::string& prompt, float temp, std::function<void(int)> token_callback);

private:
    std::string model_path_;
    int n_ctx_;
    int n_threads_;

    llama_model* model_ = nullptr;
    llama_context* ctx_ = nullptr;
};

#endif // LLAMA_GENERATOR_H
