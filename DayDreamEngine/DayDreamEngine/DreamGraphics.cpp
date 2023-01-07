
#include <pch.h>


#ifdef DREAM_OPENGL
#include "DreamGLGraphics.h"
#endif

#ifdef DREAM_DX
#include "DreamDXGraphics.h"
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

#ifdef DREAM_DX
		myGrpahics = new DreamDXGraphics();
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

#ifdef DREAM_DX
		link = new DreamDXShaderLinker();
#endif

#ifdef DREAM_VULKAN
		link = new DreamVulkanShaderLinker();
#endif
	}
	return link;
}

DreamGraphics::~DreamGraphics()
{
}

DreamShaderLinker::DreamShaderLinker()
{
}
