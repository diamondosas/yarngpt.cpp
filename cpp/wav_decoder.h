#ifndef WAV_DECODER_H
#define WAV_DECODER_H

#include <string>
#include <vector>

// Forward declare torch jit module
namespace torch {
namespace jit {
struct Module;
}
}

class WavDecoder {
public:
    WavDecoder(const std::string& model_path);
    ~WavDecoder();

    bool load();

    // Decode a sequence of integer codes into an audio waveform.
    // Returns a vector of PCM float samples (24kHz, Mono).
    std::vector<float> decode(const std::vector<int>& codes);

private:
    std::string model_path_;
    torch::jit::Module* module_ = nullptr;
};

#endif // WAV_DECODER_H
