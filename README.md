# YarnGPT C++

This repository is a C++ port of the original [YarnGPT](https://github.com/saheedniyi02/yarngpt) project, designed for use with GGML.

## Overview

This project has been restructured as a robust C++ skeleton built using Test-Driven Development (TDD). The core logic (Python wrappers, torch dependencies) has been stripped and replaced with C++ class stubs and definitions intended to mirror the functionality while integrating tightly with high-performance `ggml` tensors and contexts.

All the essential functions like `process_text`, `create_prompt`, and `get_audio` have been stubbed out inside `src/yarngpt.cpp`.

### Features
* **GGML Ready Interface:** Designed from the ground up for hypothetical linking with `ggml`.
* **GTest Integration:** Includes failing unit tests that define the contract of the tokenizer.
* **CMake Pipeline:** Simple to build across platforms.
* **Voice Presets:** Retains the original `default_speakers` and `default_speakers_local` JSON structures.
* **GitHub Actions:** Automatically compiles the library and bundles the binaries/tests into an artifact on each push.

## Setup and Compilation

You need `CMake` (3.14+) and a C++17 compatible compiler to build this project. GTest is automatically fetched during the CMake generation step.

```bash
# Clone the repo
git clone https://github.com/YOUR_USERNAME/yarngpt.cpp
cd yarngpt.cpp

# Create a build directory
mkdir build
cd build

# Configure and compile
cmake ..
make
```

## Running the Tests (TDD)

Since this project follows TDD, the provided test suite intentionally fails until the stub methods in `src/yarngpt.cpp` are implemented. To run the test suite and see the missing requirements:

```bash
cd build
ctest --output-on-failure
# Or run the executable directly:
./test_yarngpt
```

## Integrating GGML

Currently, the stubs accept and return standard C++ library components like `std::vector` and `std::string`. The `AudioTokenizer` class contains a `ggml_context*` placeholder member variable.

To complete the implementation:
1. Link the GGML library in the `CMakeLists.txt` file (commented out by default).
2. Complete the logic inside `src/yarngpt.cpp` by managing `ggml_tensor` operations, loading model weights into the `ggml_context`, and utilizing `llama.cpp` structures if necessary for inference.
