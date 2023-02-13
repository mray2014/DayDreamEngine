#include "DreamMaterial.h"
#include "DreamGraphics.h"

void DreamMaterial::Bind()
{
	if (graphicsPipeLine) {
		graphicsPipeLine->UpdateUniform<MatDataComponent>("MaterialData", data);
		graphicsPipeLine->BindShaderLink();
	}
}

void DreamMaterial::UnBind()
{
	if (graphicsPipeLine) {
		graphicsPipeLine->UnBindShaderLink();
	}
}
