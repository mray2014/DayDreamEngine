#include "DreamTexture.h"
#include "DreamGraphics.h"

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

DreamTexture::DreamTexture(const char* filePath, TextureType texType) : type(texType)
{
	stbi_set_flip_vertically_on_load(true);
	stbi_uc* pixels = stbi_load(filePath, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

	textureSize = (texWidth * texHeight) * 4;

	DreamGraphics* graphics = DreamGraphics::GetInstance();

	//DreamBuffer* pixelBuffer = graphics->GenerateBuffer(BufferType::TextureBuffer, pixels, textureSize);
	image = graphics->GenerateTexture(pixels, texWidth, texHeight);

	stbi_image_free(pixels);
}

DreamTexture::~DreamTexture()
{
}
