#include "DreamGLGraphics.h"
#include <DreamFileIO.h>
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

void DreamGLGraphics::GenerateVertexArray(size_t numOfBuffers, size_t& VBO)
{
	glGenVertexArrays(numOfBuffers, &VBO);
}

void DreamGLGraphics::GenerateBuffer(size_t numOfBuffers, size_t& VBO)
{
	glGenBuffers(numOfBuffers, &VBO);
}

int vertexArrayIndex = -1;
int vertexArrayStrideCount = 0;
void DreamGLGraphics::BindVertexArray(size_t& VBO)
{
	glBindVertexArray(VBO);
}

void DreamGLGraphics::BindBuffer(BufferType type, size_t& VBO)
{
	unsigned int buffType = -1;

	switch (type) {
	case BufferType::ArrayBuffer:
		buffType = GL_ARRAY_BUFFER;
		break;
	case BufferType::ElementArrayBuffer:
		buffType = GL_ELEMENT_ARRAY_BUFFER;
		break;
	}

	if (buffType != -1) {
		glBindBuffer(buffType, VBO);
	}
	else {
		printf("ERROR: Invalid Buffer Type");
	}
	
}

void DreamGLGraphics::CopyBufferData(BufferType type, size_t numOfElements, void* buffer, VertexDataUsage dataUsage)
{
	unsigned int buffType = -1;

	switch (type) {
	case BufferType::ArrayBuffer:
		buffType = GL_ARRAY_BUFFER;
		break;
	case BufferType::ElementArrayBuffer:
		buffType = GL_ELEMENT_ARRAY_BUFFER;
		break;
	}

	if (buffType == -1) {
		printf("ERROR: Invalid Buffer Type");
	}


	switch (dataUsage) {
	case VertexDataUsage::StreamDraw:
		glBufferData(buffType, numOfElements, buffer, GL_STREAM_DRAW);
		break;
	case VertexDataUsage::StaticDraw:
		glBufferData(buffType, numOfElements, buffer, GL_STATIC_DRAW);
		break;
	case VertexDataUsage::DynamicDraw:
		glBufferData(buffType, numOfElements, buffer, GL_DYNAMIC_DRAW);
		break;
	}
	
}

void DreamGLGraphics::AddVertexAttributePointer(int size, unsigned int dataType, bool shouldNormalize, unsigned int sizeOf)
{
	vertexArrayIndex++;
	glEnableVertexAttribArray(vertexArrayIndex);
	//TODO: Take out GL_FLOAT
	glVertexAttribPointer(vertexArrayIndex, size, GL_FLOAT, shouldNormalize ? GL_FALSE:GL_TRUE, sizeOf, (void*)vertexArrayStrideCount);
	vertexArrayStrideCount += sizeOf;
}

void DreamGLGraphics::UnBindVertexArray()
{
	glBindVertexArray(0);
	vertexArrayIndex = -1;
	vertexArrayStrideCount = 0;
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

void DreamGLGraphics::Draw() {
	//glUniformMatrix4fv(0, 1, GL_FALSE, &obj->worldPos[0][0]);
	///glUniformMatrix4fv(1, 1, GL_FALSE, &cam->viewMatrix[0][0]);
	//glUniformMatrix4fv(2, 1, GL_FALSE, &cam->ProjectMatrix[0][0]);
	//glUniform1f(7, time);
	//glUniform3f(8, cam->camPos.x, cam->camPos.y, cam->camPos.z);
	//glUniform1i(10, obj->hasTex);
	//glUniform4f(13, gameObjs[i]->objMesh.specular.x, gameObjs[i]->objMesh.specular.y, gameObjs[i]->objMesh.specular.z, gameObjs[i]->objMesh.specular.w);
	//if (obj->hasTex)
	//{
	//	glBindTexture(GL_TEXTURE_2D, obj->GetTexId());
	//}
	//if (obj->GetTag() == "Light")
	//{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//}
	//glBindVertexArray(obj->GetVertArr());
	
	//TODO: Need to figure out how to know when to draw using indices or not
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

	//glBindVertexArray(0);
	//glBindTexture(GL_TEXTURE_2D, 0);
	//if (obj->GetTag() == "Light")
	//{
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//}
}



unsigned int DreamGLGraphics::LoadShader(const char* file, ShaderType shaderType) {
	using namespace std;

	GLuint prog = 0;

	if (DreamFileIO::OpenFileRead(file))
	{
		char* fileContent = nullptr;

		if (DreamFileIO::ReadFullFile(&fileContent)) {

			switch (shaderType) {
			case ShaderType::Vertex: {
				prog = glCreateShader(GL_VERTEX_SHADER);
				break;
			}
			case ShaderType::Pixel: {
				prog = glCreateShader(GL_FRAGMENT_SHADER);
				break;
			}
			case ShaderType::Geometry: {
				prog = glCreateShader(GL_GEOMETRY_SHADER);
				break;
			}
			case ShaderType::Compute: {
				//prog = glCreateShader(GL_COMPUTE_SHADER);
				break;
			}
			}
			
			if (prog != 0) {
				glShaderSource(prog, 1, &fileContent, 0);

				glCompileShader(prog);

				GLint result;
				glGetShaderiv(prog, GL_COMPILE_STATUS, &result);

				if (result == 0)
				{

					glGetShaderiv(prog, GL_INFO_LOG_LENGTH, &result);
					GLchar* debug = new GLchar[result];

					glGetShaderInfoLog(prog, result, 0, debug);
					printf(debug);
					glDeleteShader(prog);
					delete[] debug; debug = nullptr;
				}
			}
		}
		if (fileContent) { delete fileContent; fileContent = nullptr; }

		DreamFileIO::CloseFileRead();
	}
	return prog;
}

GLuint shaderProgramCreation = -1;
void DreamGLGraphics::StartShaderProgramCreation()
{
	if (shaderProgramCreation == -1) {
		shaderProgramCreation = glCreateProgram();
	}
	else {
		printf("ERROR: Shader Program creation in progress!");
	}
}

void DreamGLGraphics::AttachShader(unsigned int shader)
{
	if (shaderProgramCreation != -1) {
		glAttachShader(shaderProgramCreation, shader);
	}
	else {
		printf("ERROR: No Shader Program created!");
	}
}

unsigned int DreamGLGraphics::FinishShaderProgramCreation()
{
	if (shaderProgramCreation != -1) {
		glLinkProgram(shaderProgramCreation);
		unsigned int store = shaderProgramCreation;
		shaderProgramCreation = -1;

		return store;
	}
	else {
		printf("ERROR: No Shader Program created!");
	}
}

void DreamGLGraphics::SetShader(unsigned int shaderProg)
{
	glUseProgram(shaderProg);
}
