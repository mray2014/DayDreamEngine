#include "DreamShaderLinker.h"
#include "DreamGraphics.h"

void DreamShaderLinker::UpdateBuffer(DreamBuffer* buffer, void* bufferData, size_t bufSize) {
	DreamGraphics* graphics = DreamGraphics::GetInstance();

	if (graphics) {
		graphics->UpdateBufferData(buffer, bufferData, bufSize);
	}
}