#include "DreamMaterial.h"
#include "DreamGraphics.h"

DreamMaterial::DreamMaterial(DreamShaderLinker* pipeline) {
	graphicsPipeLine = pipeline;
}

void DreamMaterial::StoreTexture(const std::string& texName, DreamTexture* texture)
{
	textureMap[texName] = texture;
}

void DreamMaterial::LoadUniformIndexs(UniformIndexStore& store) {
	graphicsPipeLine->GenerateUniformIndexs(store);
}

void DreamMaterial::Bind(UniformIndexStore& indexStore)
{
	if (graphicsPipeLine) {
		graphicsPipeLine->BindShaderLink(indexStore, textureMap);
	}
}

void DreamMaterial::UnBind()
{
	if (graphicsPipeLine) {
		graphicsPipeLine->UnBindShaderLink();
	}
}
