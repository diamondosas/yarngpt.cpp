package yarngpt

/*
#cgo CFLAGS: -I../../../cpp
#cgo LDFLAGS: -L../../../cpp/build -lyarngpt -lstdc++
#include "yarngpt.h"
#include <stdlib.h>

// Go callback forward declaration
extern void goAudioCallback(float* audio_data, int num_samples, void* user_data);

// Wrapper for the callback so C code can call into Go
void cgo_audio_cb_wrapper(const float* audio_data, int num_samples, void* user_data);
*/
import "C"
import (
	"errors"
	"runtime/cgo"
	"unsafe"
)

type Engine struct {
	ctx *C.yarngpt_context_t
}

type AudioCallback func(audio []float32)

func NewEngine(modelPath, wavPath string) (*Engine, error) {
	cModelPath := C.CString(modelPath)
	cWavPath := C.CString(wavPath)
	defer C.free(unsafe.Pointer(cModelPath))
	defer C.free(unsafe.Pointer(cWavPath))

	config := C.yarngpt_config_t{
		model_path:     cModelPath,
		wav_model_path: cWavPath,
		n_threads:      4,
		n_ctx:          2048,
	}

	ctx := C.yarngpt_init(config)
	if ctx == nil {
		return nil, errors.New("failed to initialize C++ Yarngpt context")
	}

	return &Engine{ctx: ctx}, nil
}

func (e *Engine) Close() {
	if e.ctx != nil {
		C.yarngpt_free(e.ctx)
		e.ctx = nil
	}
}

//export goAudioCallback
func goAudioCallback(audioData *C.float, numSamples C.int, userData unsafe.Pointer) {
	handle := *(*cgo.Handle)(userData)
	cb := handle.Value().(AudioCallback)

	// Convert C array to Go slice
	slice := unsafe.Slice((*float32)(unsafe.Pointer(audioData)), int(numSamples))

	// Create a copy because C memory might be freed immediately
	audioCopy := make([]float32, len(slice))
	copy(audioCopy, slice)

	cb(audioCopy)
}

func (e *Engine) GenerateStream(text, speaker string, temp float32, cb AudioCallback) error {
	cText := C.CString(text)
	cSpeaker := C.CString(speaker)
	defer C.free(unsafe.Pointer(cText))
	defer C.free(unsafe.Pointer(cSpeaker))

	// Pass the Go callback via cgo.Handle
	handle := cgo.NewHandle(cb)
	defer handle.Delete()

	res := C.yarngpt_generate_stream(
		e.ctx,
		cText,
		cSpeaker,
		C.float(temp),
		C.yarngpt_audio_callback(C.cgo_audio_cb_wrapper),
		unsafe.Pointer(&handle),
	)

	if res != 0 {
		return errors.New("generation failed")
	}
	return nil
}
