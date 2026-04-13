# YarnGPT Model Conversion Scripts

Before you can run the C++ or Go implementations, you must convert the original PyTorch models.

## 1. Convert WavTokenizer to TorchScript

LibTorch (C++) requires models to be exported to TorchScript.

Run the Python script:
```bash
python export_wavtokenizer.py --config /path/to/yaml --ckpt /path/to/ckpt --output wavtokenizer.pt
```

## 2. Convert LLM to GGUF

`llama.cpp` requires the language model (SmolLM2 base) to be in GGUF format. You can use the standard `llama.cpp` conversion scripts to convert the Hugging Face repo `saheedniyi/YarnGPT2` to GGUF.

```bash
git clone https://github.com/ggerganov/llama.cpp
pip install -r llama.cpp/requirements.txt
python llama.cpp/convert_hf_to_gguf.py saheedniyi/YarnGPT2 --outfile yarngpt.gguf
```
