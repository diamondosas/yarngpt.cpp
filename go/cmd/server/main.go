package main

import (
	"encoding/json"
	"flag"
	"fmt"
	"log"
	"net/http"
	"github.com/saheedniyi02/yarngpt/go/pkg/yarngpt" // Assuming Go module is initialized
	"encoding/binary"
	"bytes"
)

var engine *yarngpt.Engine

type GenerateRequest struct {
	Text        string  `json:"text"`
	Speaker     string  `json:"speaker"`
	Temperature float32 `json:"temperature"`
}

func handleGenerate(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodPost {
		http.Error(w, "Method not allowed", http.StatusMethodNotAllowed)
		return
	}

	var req GenerateRequest
	if err := json.NewDecoder(r.Body).Decode(&req); err != nil {
		http.Error(w, err.Error(), http.StatusBadRequest)
		return
	}

	if req.Speaker == "" {
		req.Speaker = "idera"
	}
	if req.Temperature == 0 {
		req.Temperature = 0.1
	}

	w.Header().Set("Content-Type", "application/octet-stream")
	w.Header().Set("Transfer-Encoding", "chunked")

	// Flush immediately to start the chunked response
	flusher, ok := w.(http.Flusher)
	if !ok {
		http.Error(w, "Streaming unsupported!", http.StatusInternalServerError)
		return
	}

	err := engine.GenerateStream(req.Text, req.Speaker, req.Temperature, func(audio []float32) {
		// Convert float32 array to little-endian bytes and write to stream
		buf := new(bytes.Buffer)
		for _, sample := range audio {
			binary.Write(buf, binary.LittleEndian, sample)
		}
		w.Write(buf.Bytes())
		flusher.Flush()
	})

	if err != nil {
		log.Printf("Generation error: %v", err)
	}
}

func main() {
	modelPath := flag.String("model", "yarngpt.gguf", "Path to GGUF model")
	wavPath := flag.String("wav", "wavtokenizer.pt", "Path to TorchScript WavTokenizer")
	port := flag.Int("port", 8080, "Port to listen on")
	flag.Parse()

	log.Println("Initializing Engine...")
	var err error
	engine, err = yarngpt.NewEngine(*modelPath, *wavPath)
	if err != nil {
		log.Fatalf("Failed to initialize engine: %v", err)
	}
	defer engine.Close()

	http.HandleFunc("/v1/audio/speech", handleGenerate)

	// Serve UI directory
	fs := http.FileServer(http.Dir("../../ui"))
	http.Handle("/", fs)

	log.Printf("Server listening on :%d", *port)
	if err := http.ListenAndServe(fmt.Sprintf(":%d", *port), nil); err != nil {
		log.Fatal(err)
	}
}
