#include "DreamGLGraphics.h"
#include <iostream>
#include <glad\glad.h>
#include <GLFW\glfw3.h>

void WindowResizeCallBack(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}


DreamGLGraphics::DreamGLGraphics()
{
}


DreamGLGraphics::~DreamGLGraphics()
{
}
void DreamGLGraphics::InitGraphics()
{
	// Init
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}
DreamPointer* DreamGLGraphics::CreateWindow(int width, int height, const char* title) {
	GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);

	if (window == NULL)
	{
		printf("Failed to create a window");
		glfwTerminate();
		return nullptr;
	}
	return new DreamPointer(window);
}
void DreamGLGraphics::CreateContext(DreamPointer* window) {
	// Creat Context
	glfwMakeContextCurrent((GLFWwindow*)window->GetStoredPointer());
}
void DreamGLGraphics::SetViewPort(int posX, int posY, int width, int height) {
	glViewport(posX, posY, width, height);
}

void DreamGLGraphics::SetWindowResizeCallBack(DreamPointer* window)
{
	glfwSetWindowSizeCallback((GLFWwindow*)window->GetStoredPointer(), WindowResizeCallBack);
}

bool DreamGLGraphics::CheckWindowClose(DreamPointer * window)
{
	return glfwWindowShouldClose((GLFWwindow*)window->GetStoredPointer());
}

void DreamGLGraphics::FindCorrectFunctionPointers()
{
	InitGlad();
}

void DreamGLGraphics::SetScreenClearColor(DreamMath::DreamVector4 color)
{
	glClearColor(color.x, color.y, color.z, color.w);
}

void DreamGLGraphics::SetScreenClearColor(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

void DreamGLGraphics::ClearScreen()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void DreamGLGraphics::SwapBuffers(DreamPointer * window)
{
	glfwSwapBuffers((GLFWwindow*)window->GetStoredPointer());
}

void DreamGLGraphics::CheckInputs()
{
	glfwPollEvents();
}

void DreamGLGraphics::GenerateBuffer(size_t numOfBuffers, size_t& VBO)
{
	glGenBuffers(numOfBuffers, &VBO);
}

void DreamGLGraphics::BindBuffer(BufferType type, size_t& VBO)
{
	switch (type) {
	case BufferType::ArrayBuffer:
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		break;
	}
	
}

void DreamGLGraphics::CopyVertexBufferData(size_t numOfVerts, void* verts, VertexDataUsage dataUsage)
{
	switch (dataUsage) {
	case VertexDataUsage::StreamDraw:
		glBufferData(GL_ARRAY_BUFFER, numOfVerts, verts, GL_STREAM_DRAW);
		break;
	case VertexDataUsage::StaticDraw:
		glBufferData(GL_ARRAY_BUFFER, numOfVerts, verts, GL_STATIC_DRAW);
		break;
	case VertexDataUsage::DynamicDraw:
		glBufferData(GL_ARRAY_BUFFER, numOfVerts, verts, GL_DYNAMIC_DRAW);
		break;
	}
	
}

void DreamGLGraphics::TerminateGraphics()
{
	glfwTerminate();
}

void DreamGLGraphics::InitGlad()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to Init GLAD");
	}
}
