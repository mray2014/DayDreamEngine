#pragma once
#include "DreamBuffer.h"

enum ShaderType {
	VertexShader,
	PixelShader,
	TessalationShader,
	GeometryShader,
	ComputeShader,
};

class DreamShader {
public:
	DreamShader();
	~DreamShader();
	DreamShader(const wchar_t* file, ShaderType shaderType);

	virtual void BindShaderData() = 0;
	DreamPointer GetShaderPtr() {
		return shaderPtr;
	}
	ShaderType GetShaderType() {
		return type;
	}

protected:
	ShaderType type;
	DreamPointer shaderPtr;
};

class VertexDreamShader : public DreamShader {
public:
	VertexDreamShader() {
		type = ShaderType::VertexShader;
	}
	~VertexDreamShader();
	VertexDreamShader(const wchar_t* file);

	void BindShaderData() override;

	DreamBuffer* GetInputLayout() {
		return layout;
	}
private:
	DreamBuffer* layout = nullptr;
};

class PixelDreamShader : public DreamShader {
public:
	PixelDreamShader() {
		type = ShaderType::PixelShader;
	}
	~PixelDreamShader();
	PixelDreamShader(const wchar_t* file);

	void BindShaderData() override;
};