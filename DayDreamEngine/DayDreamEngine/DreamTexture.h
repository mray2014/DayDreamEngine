#pragma once
#include "DreamBuffer.h"

class DreamTexture {

public:
	DreamTexture(const char* filePath);
	~DreamTexture();

	DreamPointer* GetTexturePointer() {
		return image;
	}

	int texWidth, texHeight, texChannels, textureSize;
private:

	DreamBuffer* textureBuffer;
	DreamPointer* image;
};