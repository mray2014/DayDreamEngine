#pragma once
#include "DreamBuffer.h"

enum class TextureType {
	Texture1D,
	Texture2D,
	Texture3D,
};

class DreamTexture {

public:
	DreamTexture(const char* filePath, TextureType texType = TextureType::Texture2D);
	~DreamTexture();

	DreamPointer* GetTexturePointer() {
		return image;
	}
	TextureType GetTextureType() {
		return type;
	}
	int texWidth, texHeight, texChannels, textureSize;
private:

	TextureType type;
	DreamBuffer* textureBuffer;
	DreamPointer* image;
};