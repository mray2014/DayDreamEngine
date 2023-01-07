#pragma once

enum BufferType {
	//VertexArray,
	ArrayBuffer,
	ElementArrayBuffer
};

struct DreamPointer {
	DreamPointer() {

	}

	DreamPointer(void* ptr) {
		store.ptr = ptr;
	}

	DreamPointer(size_t handle){
		store.handle = handle;
	}

	const void* GetStoredPointer() {
		return store.ptr;
	}
	size_t& GetStoredHandle() {
		return store.handle;
	}

private:
	union StoreUnion
	{
		void* ptr;
		size_t handle;
	};
	StoreUnion store;
};

class DreamBuffer {
public:
	DreamBuffer() {
		info = DreamPointer();
	}
	DreamBuffer(void* newPtr, size_t numOfBuffs = 1, size_t* s = nullptr, size_t* o = nullptr);
	DreamBuffer(size_t handle, size_t numOfBuffs = 1, size_t* s = nullptr, size_t* o = nullptr);

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

private:


	DreamPointer info;
	size_t numOfBuffers;

	size_t* stride;
	size_t* offset;
};