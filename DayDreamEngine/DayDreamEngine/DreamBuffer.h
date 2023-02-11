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

	DreamPointer(void* ptr, size_t ptrSize = 0) {
		store.ptr = ptr;
		ptrBlockSize = ptrSize;
	}

	DreamPointer(size_t handle, size_t ptrSize = 0){
		store.handle = handle;
		ptrBlockSize = ptrSize;
	}

	const void* GetStoredPointer() {
		return store.ptr;
	}
	size_t& GetStoredHandle() {
		return store.handle;
	}

	size_t GetPtrBlockSize() {
		return ptrBlockSize;
	}

private:
	union StoreUnion
	{
		void* ptr;
		size_t handle;
	};
	StoreUnion store;
	size_t ptrBlockSize;
};

class DreamBuffer {
public:
	DreamBuffer() {
		info = DreamPointer();
	}
	DreamBuffer(void* newPtr, BufferType t = BufferType::ArrayBuffer, size_t ptrBlockSize = 0, size_t numOfBuffs = 1, size_t* s = nullptr, size_t* o = nullptr);
	DreamBuffer(size_t handle, BufferType t = BufferType::ArrayBuffer, size_t ptrBlockSize = 0, size_t numOfBuffs = 1, size_t* s = nullptr, size_t* o = nullptr);

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
	size_t numOfBuffers;

	size_t* stride;
	size_t* offset;
};