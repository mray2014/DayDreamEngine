#include "DreamMaterial.h"


DreamMaterial::DreamMaterial(DreamShaderLinker* pipeline) {
	graphicsPipeLine = pipeline;
}

void DreamMaterial::LoadUniformIndexs(UniformIndexStore& store) {
	graphicsPipeLine->GenerateUniformIndexs(store);
}

void DreamMaterial::Bind(UniformIndexStore& indexStore)
{
	if (graphicsPipeLine) {
		graphicsPipeLine->BindShaderLink(indexStore);
	}
}

void DreamMaterial::UnBind()
{
	if (graphicsPipeLine) {
		graphicsPipeLine->UnBindShaderLink();
	}
}
