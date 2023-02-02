
#include <pch.h>


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
		return (float)myGrpahics->width / (float)myGrpahics->height;
	}
}

DreamGraphics::~DreamGraphics()
{
}

DreamShaderLinker::DreamShaderLinker()
{
}
