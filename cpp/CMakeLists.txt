cmake_minimum_required(VERSION 3.10)
project(yarngpt)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ideally, we would find LibTorch and llama.cpp here.
# find_package(Torch REQUIRED)
# add_subdirectory(llama.cpp)

# Core Library
add_library(yarngpt SHARED
    yarngpt.cpp
    llama_generator.cpp
    wav_decoder.cpp
)

# target_link_libraries(yarngpt ${TORCH_LIBRARIES} llama)

# CLI App
add_executable(yarngpt-cli cli/main.cpp)
target_link_libraries(yarngpt-cli yarngpt)
