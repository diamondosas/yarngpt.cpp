# YarnGPT Golang Server & Bindings

This directory provides the Golang wrapper around the C++ core engine and a streaming HTTP server.

## Structure

- **`pkg/yarngpt`**: The Golang wrapper using `cgo`. It imports `yarngpt.h` and links against `libyarngpt`.
- **`cmd/server`**: The HTTP streaming server, similar to `whisper-server`.
- **`example/`**: Simple scripts demonstrating how to use the Go package programmatically.

## Building the Server

First, ensure the C++ shared library (`libyarngpt.so` or `libyarngpt.dylib`) is built and accessible in your library path.

```bash
cd cmd/server
go build -o yarngpt-server main.go
```

## Running the Server

```bash
./yarngpt-server --model /path/to/yarngpt.gguf --wav /path/to/wavtokenizer.pt --port 8080
```
