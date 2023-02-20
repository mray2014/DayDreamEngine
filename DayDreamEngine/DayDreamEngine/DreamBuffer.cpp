#include "DreamBuffer.h"
#include <memory>

DreamBuffer::DreamBuffer(void* newPtr, BufferType t, size_t ptrBlockSize, size_t numOfBuffs, size_t* s, size_t* o) {
	type = t;
	info = DreamPointer(newPtr, ptrBlockSize);
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

DreamBuffer::DreamBuffer(size_t handle, BufferType t, size_t ptrBlockSize, size_t numOfBuffs, size_t* s, size_t* o) {
	type = t;
	info = DreamPointer(handle, ptrBlockSize);
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

DreamBuffer::DreamBuffer(size_t handle, void* newPtr, BufferType t, size_t ptrBlockSize, size_t numOfBuffs, size_t* s, size_t* o) {
	type = t;
	info = DreamPointer(handle, newPtr, ptrBlockSize);
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