#pragma once

enum BufferType {
	//VertexArray,
	ArrayBuffer,
	ElementArrayBuffer,
	UniformBuffer
};

struct DreamPointer {
	DreamPointer() {

	}

	DreamPointer(void* p, size_t ptrSize = 0) {
		ptr = p;
		ptrBlockSize = ptrSize;
	}

	DreamPointer(size_t hand, size_t ptrSize = 0){
		handle = hand;
		ptrBlockSize = ptrSize;
	}

	DreamPointer(size_t hand, void* p, size_t ptrSize = 0) {
		ptr = p;
		handle = hand;
		ptrBlockSize = ptrSize;
	}

	void* GetStoredPointer() {
		return ptr;
	}
	size_t& GetStoredHandle() {
		return handle;
	}

	size_t GetPtrBlockSize() {
		return ptrBlockSize;
	}

private:
	void* ptr = nullptr;
	size_t handle = -1;
	size_t ptrBlockSize = 0;
};

class DreamBuffer {
public:
	DreamBuffer() {
		info = DreamPointer();
	}
	DreamBuffer(void* newPtr, BufferType t = BufferType::ArrayBuffer, size_t ptrBlockSize = 0, size_t numOfBuffs = 1, size_t* s = nullptr, size_t* o = nullptr);
	DreamBuffer(size_t handle, BufferType t = BufferType::ArrayBuffer, size_t ptrBlockSize = 0, size_t numOfBuffs = 1, size_t* s = nullptr, size_t* o = nullptr);
	DreamBuffer(size_t handle, void* newPtr, BufferType t = BufferType::ArrayBuffer, size_t ptrBlockSize = 0, size_t numOfBuffs = 1, size_t* s = nullptr, size_t* o = nullptr);

	~DreamBuffer() {
		if (stride) {
			delete stride;
			stride = nullptr;
		}

		if (offset) {
			delete offset;
			offset = nullptr;
		}
	}

	 DreamPointer GetBufferPointer() {
		return info;
	}

	size_t GetNumOfBuffers() {
		return numOfBuffers;
	}

	const size_t* GetBufferStrides() {
		return stride;
	}

	const size_t* GetBufferOffsets() {
		return offset;
	}

	BufferType GetBufferType() {
		return type;
	}

private:

	BufferType type;
	DreamPointer info;
	size_t numOfBuffers = 0;

	size_t* stride = nullptr;
	size_t* offset = nullptr;
};