#pragma once
#include "DreamBuffer.h"
#include <DreamMath.h>
#include <unordered_map>
#include <string>
#include "DreamLight.h"

enum ShaderType {
	VertexShader,
	PixelShader,
	TessalationShader,
	GeometryShader,
	ComputeShader,
};

#pragma region Uniform Information
using UniformMembers = std::unordered_map<std::string, int>;

struct UniformInfo {
	std::vector<DreamBuffer*> buffers;
	int bindingIndex;
	int uniformSize;
	int maxIndex;
	UniformMembers uniformMembers;

	UniformInfo();
	UniformInfo(int index, int size, UniformMembers members);

	int AddUniformBuffer();
	DreamBuffer* GetUniformBuffer(int index);
};

using UniformList = std::unordered_map<std::string, UniformInfo>;

using UniformIndexStore = std::unordered_map<std::string, unsigned int>;

using UniformBindingPoints = std::unordered_map<std::string, unsigned int>;


enum UniformBufferLayout {
	ConstantData = 0,
	MaterialData = 1
};
struct ConstantUniformData {
	DreamMath::DreamMatrix4X4 viewMat;
	DreamMath::DreamMatrix4X4 projMat;
	float totalTime;

	UniformMembers GetMemberData() {
		UniformMembers members;
		members["viewMat"] = sizeof(DreamMath::DreamMatrix4X4);
		members["projMat"] = sizeof(DreamMath::DreamMatrix4X4);
		members["time"] = sizeof(float);

		return members;
	}
};

struct LightUniformData {
	DreamDirectionalLight light;
	float ambient;

	UniformMembers GetMemberData() {
		UniformMembers members;
		members["light"] = sizeof(DreamDirectionalLight);
		members["ambient"] = sizeof(float);

		return members;
	}
};
#pragma endregion

class DreamShader {
public:
	DreamShader();
	DreamShader(ShaderType t, DreamPointer ptr, UniformList uniforms, bool hasMat) {
		type = t;
		shaderPtr = ptr;
		shaderUniforms = uniforms;
		hasMaterialUniform = hasMat;

		if (type == VertexShader) {
			//CreateVertexInputLayout();
		}
	}
	~DreamShader();

	virtual void BindShaderData();
	DreamPointer GetShaderPtr() {
		return shaderPtr;
	}
	ShaderType GetShaderType() {
		return type;
	}
	DreamBuffer* GetInputLayout() {
		return layout;
	}

	UniformList  shaderUniforms;

protected:
	void CreateVertexInputLayout();

	ShaderType type;
	DreamPointer shaderPtr;
	bool hasMaterialUniform = false;

private:
	DreamBuffer* layout = nullptr;
};

//class VertexDreamShader : public DreamShader {
//public:
//	VertexDreamShader() {
//		type = ShaderType::VertexShader;
//	}
//	~VertexDreamShader();
//	VertexDreamShader(const wchar_t* file);
//
//	void BindShaderData() override;
//
//	DreamBuffer* GetInputLayout() {
//		return layout;
//	}
//private:
//	DreamBuffer* layout = nullptr;
//};
//
//class PixelDreamShader : public DreamShader {
//public:
//	PixelDreamShader() {
//		type = ShaderType::PixelShader;
//	}
//	~PixelDreamShader();
//	PixelDreamShader(const wchar_t* file);
//
//	void BindShaderData() override;
//};
