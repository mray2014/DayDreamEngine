#pragma once
#include <string>
#include <unordered_map>
#include <DreamMath.h>
#include "DreamShaderLinker.h"

//class DreamShaderLinker;

struct MatDataComponent {
	DreamMath::DreamMatrix4X4 worldMat;
	DreamMath::DreamVector4 matColor;
	float specular;

	MatDataComponent() {
		worldMat = DreamMath::DreamMatrix4X4::Identity();
		matColor = DreamMath::DreamVector4(1.0f, 1.0f, 1.0f, 1.0f);
		specular = 1.0f;
	};
};

class DreamMaterial {
public:
	DreamMaterial() {
		graphicsPipeLine = nullptr;
	}

	DreamMaterial(DreamShaderLinker* pipeline);

	void LoadUniformIndexs(UniformIndexStore& store);
	void Bind(UniformIndexStore& indexStore);
	void UnBind();

private:
	DreamShaderLinker* graphicsPipeLine;

public:
	template<typename T>
	inline void UpdateUniformData(T& data, std::string name, unsigned index)
	{
		if (graphicsPipeLine) {
			graphicsPipeLine->UpdateUniform<T>(name, data, index);
		}
	}
};