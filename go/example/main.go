package main

import (
	"fmt"
	"log"

	"github.com/saheedniyi02/yarngpt/go/pkg/yarngpt" // Assuming this is placed correctly in GOPATH or Go module
)

func main() {
	fmt.Println("Initializing YarnGPT Engine...")
	engine, err := yarngpt.NewEngine("../../models/yarngpt.gguf", "../../models/wavtokenizer.pt")
	if err != nil {
		log.Fatalf("Error: %v", err)
	}
	defer engine.Close()

	fmt.Println("Generating Audio...")
	var totalSamples int
	err = engine.GenerateStream("Hello, world! This is a test of the Go wrapper.", "idera", 0.1, func(audio []float32) {
		fmt.Printf("Received chunk of size: %d\n", len(audio))
		totalSamples += len(audio)
	})

	if err != nil {
		log.Fatalf("Generation error: %v", err)
	}

	fmt.Printf("Done! Generated total %d samples.\n", totalSamples)
}
