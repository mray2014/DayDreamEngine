#include "DreamBuffer.h"
#include <memory>

DreamBuffer::DreamBuffer(void* newPtr, void* memPtr, BufferType t, size_t ptrBlockSize, size_t numOfBuffs, size_t* s, size_t* o) {
	type = t;
	info = DreamPointer(newPtr, ptrBlockSize);
	bufferMemoryPtr = memPtr;
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

DreamBuffer::DreamBuffer(size_t handle, void* memPtr, BufferType t, size_t ptrBlockSize, size_t numOfBuffs, size_t* s, size_t* o) {
	type = t;
	info = DreamPointer(handle, ptrBlockSize);
	bufferMemoryPtr = memPtr;
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

DreamBuffer::DreamBuffer(size_t handle, void* newPtr, void* memPtr, BufferType t, size_t ptrBlockSize, size_t numOfBuffs, size_t* s, size_t* o) {
	type = t;
	info = DreamPointer(handle, newPtr, ptrBlockSize);
	bufferMemoryPtr = memPtr;
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