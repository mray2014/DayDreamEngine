#pragma once
#include <string>
#include <unordered_map>
#include <DreamMath.h>

class DreamShaderLinker;

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

	DreamMaterial(DreamShaderLinker* pipeline) {
		graphicsPipeLine = pipeline;
	}

	void Bind(MatDataComponent& data);
	void UnBind();

private:
	DreamShaderLinker* graphicsPipeLine;
};