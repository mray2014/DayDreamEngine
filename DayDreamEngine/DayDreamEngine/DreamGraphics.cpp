
#include <pch.h>
#include <DreamInput.h>

#ifdef DREAM_OPENGL
#include "DreamGLGraphics.h"
#endif

#ifdef DREAM_DX11
#include "DreamDX11Graphics.h"
#endif

#ifdef DREAM_DX12
#include "DreamDX12Graphics.h"
#endif

#ifdef DREAM_VULKAN
#include "DreamVulkanGraphics.h"
#endif


DreamGraphics* DreamGraphics::myGrpahics = nullptr;

DreamGraphics::DreamGraphics()
{

}
void DreamGraphics::InitConstData() {
	//matConstData = GenerateBuffer(BufferType::UniformBuffer, nullptr, 1, { sizeof(ConstantUniformData) }, { 0 }, VertexDataUsage::StreamDraw);
	UniformMembers constMembers = matConstData.GetMemberData();
	constDataBufferInfo = UniformInfo(0, sizeof(ConstantUniformData), constMembers);
	constDataBufferInfo.AddUniformBuffer();

	UniformMembers lightMembers = lightData.GetMemberData();
	lightBufferInfo = UniformInfo(2, sizeof(LightUniformData), lightMembers);
	lightBufferInfo.AddUniformBuffer();

	lightData.light = DreamDirectionalLight(DreamVector3(0, 0, 1));
	lightData.ambient = 1.0f;
}

void DreamGraphics::Update()
{
	DreamBuffer* constDataBuffer = constDataBufferInfo.GetUniformBuffer(currentFrame);
	UpdateBufferData(constDataBuffer, &matConstData, sizeof(ConstantUniformData));

	DreamBuffer* lightBuffer = lightBufferInfo.GetUniformBuffer(currentFrame);
	UpdateBufferData(lightBuffer, &lightData, sizeof(LightUniformData));

	if (DreamInput::KeyDown(KeyCode::O)) {
		lightData.light.direction.x += 0.01f;
		lightData.light.direction.z -= 0.01f;
	}
	if (DreamInput::KeyDown(KeyCode::P)) {
		lightData.light.direction.x -= 0.01f;
		lightData.light.direction.z += 0.01f;
	}

	printf(lightData.light.direction.ToString().c_str());
	printf("\n");
}

DreamGraphics * DreamGraphics::GetInstance()
{
	if (myGrpahics == nullptr)
	{
#ifdef DREAM_OPENGL
		myGrpahics = new DreamGLGraphics();
#endif

#ifdef DREAM_DX11
		myGrpahics = new DreamDX11Graphics();
#endif

#ifdef DREAM_DX12
		myGrpahics = new DreamDX12Graphics();
#endif

#ifdef DREAM_VULKAN
		myGrpahics = new DreamVulkanGraphics();
#endif
	}
	return myGrpahics;
}

DreamShaderLinker* DreamGraphics::GenerateShaderLinker()
{
	DreamShaderLinker* link = nullptr;
	if (link == nullptr)
	{
#ifdef DREAM_OPENGL
		link = new DreamGLShaderLinker();
#endif

#ifdef DREAM_DX11
		link = new DreamDX11ShaderLinker();
#endif

#ifdef DREAM_DX12
		link = new DreamDX12ShaderLinker();
#endif

#ifdef DREAM_VULKAN
		link = new DreamVulkanShaderLinker();
#endif
	}
	return link;
}

float DreamGraphics::GetAspectRatio() {
	if (myGrpahics)
	{
		return  (float)myGrpahics->width / (float)myGrpahics->height;
	}
	return 1.0f;
}

DreamGraphics::~DreamGraphics()
{
}

DreamShaderLinker::DreamShaderLinker()
{
}