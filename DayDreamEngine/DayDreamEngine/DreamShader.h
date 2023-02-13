#pragma once
#include "DreamBuffer.h"
#include <unordered_map>
#include <string>

enum ShaderType {
	VertexShader,
	PixelShader,
	TessalationShader,
	GeometryShader,
	ComputeShader,
};

using UniformMembers = std::unordered_map<std::string, unsigned int>;

struct UniformInfo {
	DreamBuffer* buffer;
	unsigned int bindingIndex;
	UniformMembers uniformMembers;

	UniformInfo() {
		buffer = nullptr;
		bindingIndex = -1;
	}

	UniformInfo(DreamBuffer* buf, unsigned int index, UniformMembers members) {
		buffer = buf;
		bindingIndex = index;
		uniformMembers = members;
	}
};

using UniformList = std::unordered_map<std::string, UniformInfo>;
class DreamShader {
public:
	DreamShader();
	DreamShader(ShaderType t, DreamPointer ptr, UniformList uniforms, bool hasMat) {
		type = t;
		shaderPtr = ptr;
		shaderUniforms = uniforms;
		hasMaterialUniform = hasMat;

		if (type == VertexShader) {
			CreateVertexInputLayout();
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
