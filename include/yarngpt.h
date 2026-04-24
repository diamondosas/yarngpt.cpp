#pragma once

#include <string>
#include <vector>
#include <map>
#include <cstdint>
#include <memory>

// Forward declarations for hypothetical GGML types to keep the interface GGML-friendly
struct ggml_context;
struct ggml_tensor;

namespace yarngpt {

// Struct to represent a parsed speaker from the JSON files
struct SpeakerData {
    std::string name;
    std::string text;
    std::vector<float> audio_features; // Placeholder for what might be extracted
};

// Base class for AudioTokenizer
class AudioTokenizer {
public:
    AudioTokenizer(const std::string& tokenizer_path,
                   const std::string& wav_tokenizer_model_path,
                   const std::string& wav_tokenizer_config_path);
    virtual ~AudioTokenizer();

    // Utility methods
    virtual std::string get_speaker_path(const std::string& speaker_name) const;
    virtual SpeakerData load_speaker(const std::string& path) const;
    virtual SpeakerData load_default_speaker(const std::string& name) const;

    // Core methods mapping to Python logic
    virtual std::string process_text(const std::string& text) const;
    virtual std::string create_audio_prompt(const std::vector<std::string>& words) const;
    virtual std::string create_prompt(const std::string& text, const std::string& speaker_name = "idera") const;

    // Tokenization and code generation
    virtual std::vector<int> tokenize_prompt(const std::string& prompt) const;
    virtual std::vector<float> get_audio(const std::vector<int>& discrete_code) const;

    // GGML specific or utility methods
    virtual std::vector<int> extract_integers(const std::string& s) const;
    virtual std::vector<int> get_codes(const std::string& output) const; // Could take ggml_tensor* instead of string in reality

protected:
    std::string tokenizer_path_;
    std::string wav_tokenizer_model_path_;
    std::string wav_tokenizer_config_path_;

    // Hypothetical GGML context
    ggml_context* ctx_;
};

class AudioTokenizerForLocal : public AudioTokenizer {
public:
    AudioTokenizerForLocal(const std::string& tokenizer_path,
                           const std::string& wav_tokenizer_model_path,
                           const std::string& wav_tokenizer_config_path);

    // Overridden methods
    std::string process_text(const std::string& text) const override;
    std::string create_prompt(const std::string& text, const std::string& lang, const std::string& speaker_name = "") const;
};

class AudioTokenizerV2 : public AudioTokenizer {
public:
    AudioTokenizerV2(const std::string& tokenizer_path,
                     const std::string& wav_tokenizer_model_path,
                     const std::string& wav_tokenizer_config_path);

    // Overridden/New methods
    std::string process_text(const std::string& text) const override;
    std::string get_speaker_path(const std::string& speaker_name, const std::string& dir) const;
    SpeakerData load_default_speaker(const std::string& name, const std::string& dir) const;
    std::string create_prompt(const std::string& text, const std::string& lang, const std::string& speaker_name = "") const;

    static std::string replace_tokens(const std::string& text);
    std::vector<float> resample(const std::vector<float>& audio, int sr, int target_sr) const;
    void quantize_wavtokenizer(const std::string& path) const;
    std::string create_asr_prompt(const std::string& audio_path) const;
    std::string get_asr_results(const std::string& output) const;
};

} // namespace yarngpt
