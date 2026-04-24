#include <gtest/gtest.h>
#include "yarngpt.h"

using namespace yarngpt;

class YarnGptTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup dummy tokenizer for tests
        tokenizer = std::make_unique<AudioTokenizer>("dummy_tok_path", "dummy_wav_model", "dummy_wav_config");
        tokenizer_local = std::make_unique<AudioTokenizerForLocal>("dummy_tok_path", "dummy_wav_model", "dummy_wav_config");
        tokenizer_v2 = std::make_unique<AudioTokenizerV2>("dummy_tok_path", "dummy_wav_model", "dummy_wav_config");
    }

    std::unique_ptr<AudioTokenizer> tokenizer;
    std::unique_ptr<AudioTokenizerForLocal> tokenizer_local;
    std::unique_ptr<AudioTokenizerV2> tokenizer_v2;
};

// --- Tests for AudioTokenizer Base ---

TEST_F(YarnGptTest, GetSpeakerPathReturnsValidPath) {
    std::string path = tokenizer->get_speaker_path("idera");
    // Expected to construct the correct path based on OS logic
    EXPECT_NE(path, "");
    EXPECT_TRUE(path.find("idera.json") != std::string::npos);
}

TEST_F(YarnGptTest, LoadSpeakerParsesJsonCorrectly) {
    SpeakerData data = tokenizer->load_speaker("dummy_path");
    // Since it's a stub, this will currently fail
    EXPECT_NE(data.name, "");
}

TEST_F(YarnGptTest, ProcessTextNormalizesInput) {
    std::string input = "Hello,   WORLD!";
    std::string processed = tokenizer->process_text(input);
    EXPECT_EQ(processed, "hello, world!");
}

TEST_F(YarnGptTest, CreateAudioPromptStructuresCorrectly) {
    std::vector<std::string> words = {"hello", "world"};
    std::string prompt = tokenizer->create_audio_prompt(words);
    EXPECT_TRUE(prompt.find("hello") != std::string::npos);
}

TEST_F(YarnGptTest, TokenizePromptReturnsIntegerVector) {
    std::string prompt = "dummy prompt";
    std::vector<int> tokens = tokenizer->tokenize_prompt(prompt);
    EXPECT_FALSE(tokens.empty());
}

TEST_F(YarnGptTest, GetAudioReturnsWaveformFloats) {
    std::vector<int> codes = {1, 2, 3};
    std::vector<float> audio = tokenizer->get_audio(codes);
    EXPECT_FALSE(audio.empty());
}

TEST_F(YarnGptTest, ExtractIntegersFindsNumbersInString) {
    std::string s = "token_123 token_456";
    std::vector<int> ints = tokenizer->extract_integers(s);
    ASSERT_EQ(ints.size(), 2);
    EXPECT_EQ(ints[0], 123);
    EXPECT_EQ(ints[1], 456);
}

// --- Tests for AudioTokenizerForLocal ---

TEST_F(YarnGptTest, LocalCreatePromptWithLang) {
    std::string prompt = tokenizer_local->create_prompt("hello", "en", "idera");
    EXPECT_NE(prompt, "");
    EXPECT_TRUE(prompt.find("en") != std::string::npos);
}

// --- Tests for AudioTokenizerV2 ---

TEST_F(YarnGptTest, V2ReplaceTokensStaticMethod) {
    std::string text = "<|audio_start|> hello <|audio_end|>";
    std::string replaced = AudioTokenizerV2::replace_tokens(text);
    EXPECT_NE(replaced, text);
}

TEST_F(YarnGptTest, V2ResampleAdjustsLength) {
    std::vector<float> audio = {0.1f, 0.2f, 0.3f, 0.4f};
    std::vector<float> resampled = tokenizer_v2->resample(audio, 16000, 8000);
    EXPECT_EQ(resampled.size(), 2);
}

TEST_F(YarnGptTest, V2CreateAsrPrompt) {
    std::string prompt = tokenizer_v2->create_asr_prompt("audio.wav");
    EXPECT_NE(prompt, "");
}
