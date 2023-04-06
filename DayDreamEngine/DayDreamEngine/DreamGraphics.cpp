
#include <pch.h>
#include <DreamInput.h>
#include <DreamTimeManager.h>
#include "DreamCameraManager.h"

#pragma comment(lib, "spirv-cross-reflectd.lib")
#pragma comment(lib, "spirv-cross-cored.lib")

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
using namespace spirv_cross;


DreamGraphics::DreamGraphics()
{

}
void DreamGraphics::LoadShaderResources(spirv_cross::Compiler& spirvCompiler, DreamShaderResources& shaderResources, bool& hasMat)
{
	UniformMembers uniformMembers;
	bool hasMatUniform = false;
	bool hasConstDataUniform = false;

	// The SPIR-V is now parsed, and we can perform reflection on it.
	spirv_cross::ShaderResources resources = spirvCompiler.get_shader_resources();

	for (auto& resource : resources.sampled_images)
	{
		std::string name = resource.name;

		//=======Grabbing uniform size and member data
		const spirv_cross::SPIRType type = spirvCompiler.get_type(resource.base_type_id); // What is the difference between base_type_ID and type_Id

		//=======Grabbing binding index of uniform
		unsigned set = spirvCompiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
		unsigned binding = spirvCompiler.get_decoration(resource.id, spv::DecorationBinding);
		printf("Texture2D Sampler %s at set = %u, binding = %u\n", resource.name.c_str(), set, binding);

		// Modify the decoration to prepare it for GLSL.
		spirvCompiler.unset_decoration(resource.id, spv::DecorationDescriptorSet);
		// Some arbitrary remapping if we want.
		spirvCompiler.set_decoration(resource.id, spv::DecorationBinding, set * 16 + binding);

		shaderResources.samplerBindings[name] = binding;
	}

	// Get all sampled images in the shader.
	for (auto& resource : resources.uniform_buffers)
	{
		std::string name = resource.name;

		//=======Grabbing uniform size and member data
		const spirv_cross::SPIRType type = spirvCompiler.get_type(resource.base_type_id); // What is the difference between base_type_ID and type_Id
		size_t structSize = spirvCompiler.get_declared_struct_size(type);

		int memberOffset = 0;
		for (int i = 0; i < type.member_types.size(); i++) {
			size_t memberSize = spirvCompiler.get_declared_struct_member_size(type, i);
			std::string memberName = spirvCompiler.get_member_name(resource.base_type_id, i);

			uniformMembers[memberName] = memberOffset;
			memberOffset += memberSize;
		}


		//=======Grabbing binding index of uniform
		unsigned set = spirvCompiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
		unsigned binding = spirvCompiler.get_decoration(resource.id, spv::DecorationBinding);
		printf("Uniform Buffer %s at set = %u, binding = %u\n", resource.name.c_str(), set, binding);

		// Modify the decoration to prepare it for GLSL.
		spirvCompiler.unset_decoration(resource.id, spv::DecorationDescriptorSet);
		// Some arbitrary remapping if we want.
		spirvCompiler.set_decoration(resource.id, spv::DecorationBinding, set * 16 + binding);

		//=======Creating buffer for uniform
		if (name == "ConstantData") {
			hasConstDataUniform = true;
		}
		else if (name == "MaterialData") {
			hasMatUniform = true;
		}

		//=======Storing uniform
		if (hasConstDataUniform) {
			shaderResources.uniforms[name] = constDataBufferInfo;
		}
		else if (name == "LightData") {
			shaderResources.uniforms[name] = lightBufferInfo;
		}
		else {
			shaderResources.uniforms[name] = UniformInfo(binding, structSize, uniformMembers);
		}
	}

	hasMat = (hasMatUniform && hasConstDataUniform);
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
	DreamCameraManager* camManager = DreamCameraManager::GetInstance();
	matConstData.viewMat = camManager->GetCurrentCam_ViewMat();
	matConstData.projMat = camManager->GetCurrentCam_ProjMat();
	matConstData.totalTime = DreamTimeManager::totalTime;

	if (DreamInput::KeyDown(KeyCode::O)) {
		lightData.light.direction.x += 0.01f;
		lightData.light.direction.z -= 0.01f;
	}
	if (DreamInput::KeyDown(KeyCode::P)) {
		lightData.light.direction.x -= 0.01f;
		lightData.light.direction.z += 0.01f;
	}

	DreamBuffer* constDataBuffer = constDataBufferInfo.GetUniformBuffer(0);
	UpdateBufferData(constDataBuffer, &matConstData, sizeof(ConstantUniformData));

	DreamBuffer* lightBuffer = lightBufferInfo.GetUniformBuffer(0);
	UpdateBufferData(lightBuffer, &lightData, sizeof(LightUniformData));
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