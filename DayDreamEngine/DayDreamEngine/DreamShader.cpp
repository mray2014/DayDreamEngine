#pragma once
#include "DreamShader.h"
#include "DreamGraphics.h"

DreamShader::DreamShader()
{
}

DreamShader::~DreamShader()
{
}

DreamShader::DreamShader(const wchar_t* file, ShaderType shaderType)
{
	type = shaderType;
	DreamGraphics* graphics = DreamGraphics::GetInstance();
	graphics->LoadShader(file, shaderType, shaderPtr);
}

void DreamShader::BindShaderData()
{
	DreamGraphics* graphics = DreamGraphics::GetInstance();
}

VertexDreamShader::~VertexDreamShader()
{
	DreamGraphics* graphics = DreamGraphics::GetInstance();
	graphics->ReleaseShader(this);
}

VertexDreamShader::VertexDreamShader(const wchar_t* file) : DreamShader(file, VertexShader)
{
	DreamGraphics* graphics = DreamGraphics::GetInstance();
	graphics->BeginVertexLayout();
	graphics->AddVertexLayoutData("POSITION", 3, 0, false, sizeof(DreamVector3));
	layout = graphics->FinalizeVertexLayout();
}

void VertexDreamShader::BindShaderData()
{
}

PixelDreamShader::~PixelDreamShader()
{
}

PixelDreamShader::PixelDreamShader(const wchar_t* file) : DreamShader(file, PixelShader)
{
}

void PixelDreamShader::BindShaderData()
{
}