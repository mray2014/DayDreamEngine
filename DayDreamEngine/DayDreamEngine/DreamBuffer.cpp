#include "DreamBuffer.h"
#include <memory>

DreamBuffer::DreamBuffer(void* newPtr, size_t numOfBuffs, size_t* s, size_t* o) {
	info = DreamPointer(newPtr);
	numOfBuffers = numOfBuffs;

	if (s) {
		stride = new size_t[numOfBuffers];
		std::memcpy(stride, s, sizeof(size_t) * numOfBuffers);
	}

	if (o) {
		offset = new size_t[numOfBuffers];
		std::memcpy(offset, o, sizeof(size_t) * numOfBuffers);
	}
}

DreamBuffer::DreamBuffer(size_t handle, size_t numOfBuffs, size_t* s, size_t* o) {
	info = DreamPointer(handle);
	numOfBuffers = numOfBuffs;

	if (s) {
		stride = new size_t[numOfBuffers];
		std::memcpy(stride, s, sizeof(size_t) * numOfBuffers);
	}

	if (o) {
		offset = new size_t[numOfBuffers];
		std::memcpy(offset, o, sizeof(size_t) * numOfBuffers);
	}
}