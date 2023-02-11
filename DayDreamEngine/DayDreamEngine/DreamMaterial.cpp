#include "DreamMaterial.h"
#include "DreamGraphics.h"

void DreamMaterial::Bind()
{
	if (graphicsPipeLine) {
		DreamGraphics::GetInstance()->UpdateBufferData(graphicsPipeLine->matDataBuffer, &data, sizeof(MatDataComponent));
		graphicsPipeLine->BindShaderLink();
	}
}

void DreamMaterial::UnBind()
{
	if (graphicsPipeLine) {
		graphicsPipeLine->UnBindShaderLink();
	}
}
