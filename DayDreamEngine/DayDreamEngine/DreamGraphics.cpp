
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

DreamGraphics::~DreamGraphics()
{
}
