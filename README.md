# YarnGPT.cpp

YarnGPT.cpp is a high-performance C++ inference engine and Golang web server for [YarnGPT](https://github.com/saheedniyi02/yarngpt), a Nigerian-Accented English Text-to-Speech model.

This project refactors the original Python implementation into a modular architecture optimized for speed, streaming, and easy deployment.

## Architecture Overview

The system is designed with clear separation of concerns, making it accessible to developers of all levels:

1. **`cpp/` (Core C++ Engine)**:
   - Uses `ggml` (via `llama.cpp`) to run the Language Model (SmolLM2 base) very efficiently on CPU/GPU.
   - Uses `LibTorch` (C++ PyTorch API) to run the `WavTokenizer` to decode generated tokens into audio waveforms.
   - Exposes a clean C-API to allow FFI (Foreign Function Interface) bindings.
   - Includes a standalone command-line executable (`yarngpt-cli`) for direct testing.

2. **`go/` (Golang Server & Wrapper)**:
   - Uses `cgo` to bind to the C++ core library.
   - Provides an HTTP Web Server that streams audio chunks in real-time as the language model generates tokens, similar to `whisper-server`.

3. **`ui/` (Web Frontend)**:
   - A clean, simple HTML/JS interface to interact with the Golang streaming server.

4. **`scripts/` (Model Conversion)**:
   - Contains Python scripts to convert the original Hugging Face PyTorch models into formats compatible with the C++ engine (GGUF for the LLM, TorchScript for the WavTokenizer).

## Getting Started

Check the `README.md` files in each directory for specific instructions:
- [Model Conversion Instructions](scripts/README.md)
- [C++ Build & CLI Instructions](cpp/README.md)
- [Golang Server Instructions](go/README.md)

## Releasing / Downloading
Once built via GitHub Actions or CMake, you can download `yarngpt-cli` (Windows/Linux/macOS) directly to test inference from the command line without setting up the Go server.
