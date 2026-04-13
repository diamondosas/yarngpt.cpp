# YarnGPT C++ Core

This directory contains the high-performance C++ implementation of the YarnGPT inference engine.

## Components

- **`llama_generator.cpp`**: Uses `llama.cpp` to load the GGUF model and generate audio tokens.
- **`wav_decoder.cpp`**: Uses LibTorch (PyTorch C++ API) to decode the tokens into 24kHz audio waveforms.
- **`yarngpt.cpp` / `yarngpt.h`**: The C-API wrapper that ties everything together and exports functions for external languages (like Go) to consume.
- **`cli/main.cpp`**: A standalone command-line application to run inference.

## Dependencies

- **CMake** >= 3.10
- **LibTorch**: Download the C++ distribution of PyTorch.
- **llama.cpp**: Included as a submodule or linked dynamically.

## Building the CLI

```bash
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=/path/to/libtorch
make
```

## Running the CLI

```bash
./yarngpt-cli --model path/to/yarngpt.gguf --wav path/to/wavtokenizer.pt --prompt "Hello world" --output out.wav
```
