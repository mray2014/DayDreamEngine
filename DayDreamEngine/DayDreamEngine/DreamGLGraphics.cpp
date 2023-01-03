#include "DreamGLGraphics.h"
#include <DreamFileIO.h>
#include <glad\glad.h>
#include <GLFW\glfw3.h>

GLFWwindow* window = nullptr;

void OnWindowResize(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}


DreamGLGraphics::DreamGLGraphics()
{
}


DreamGLGraphics::~DreamGLGraphics()
{
}

long DreamGLGraphics::InitWindow(int w, int h, const char* title) {
	// Init
	width = w;
	height = h;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(width, height, title, NULL, NULL);

	if (window == NULL)
	{
		printf("Failed to create a window");
		glfwTerminate();
		return -1;
	}
	glfwSetWindowSizeCallback(window, OnWindowResize);
	return 0;
}

long DreamGLGraphics::InitGraphics()
{
	glfwMakeContextCurrent(window);
	InitGlad();
	return 0;
}

void DreamGLGraphics::SetViewPort(int posX, int posY, int w, int h) {
	width = w;
	height = h;
	glViewport(posX, posY, width, height);
}

bool DreamGLGraphics::CheckWindowClose()
{
	return glfwWindowShouldClose(window);
}

void DreamGLGraphics::ClearScreen()
{
	glClearColor(clearScreenColor.x, clearScreenColor.y, clearScreenColor.z, clearScreenColor.w);
	glClear(GL_COLOR_BUFFER_BIT);
}

void DreamGLGraphics::SwapBuffers()
{
	glfwSwapBuffers(window);
}

void DreamGLGraphics::CheckInputs()
{
	glfwPollEvents();
}

void DreamGLGraphics::GenerateBuffer(BufferType type, size_t& VBO, size_t numOfBuffers, void* bufferData, size_t numOfElements, VertexDataUsage dataUsage)
{
	unsigned int buffType = -1;
	unsigned int drawType = -1;

	switch (type) {
	case BufferType::VertexArray: {
		glGenVertexArrays(numOfBuffers, &VBO);
		return;
	}
	case BufferType::ArrayBuffer: {
		glGenBuffers(numOfBuffers, &VBO);
		buffType = GL_ARRAY_BUFFER;
		break;
	}
	case BufferType::ElementArrayBuffer: {
		glGenBuffers(numOfBuffers, &VBO);
		buffType = GL_ELEMENT_ARRAY_BUFFER;
		break;
	}
	}

	if (buffType == -1) {
		printf("ERROR: Invalid Buffer Type");
		return;
	}

	switch (dataUsage) {
	case VertexDataUsage::StreamDraw:
		drawType = GL_STREAM_DRAW;
		break;
	case VertexDataUsage::StaticDraw:
		drawType = GL_STATIC_DRAW;
		break;
	case VertexDataUsage::DynamicDraw:
		drawType = GL_DYNAMIC_DRAW;		
		break;
	}

	if (buffType == -1) {
		printf("ERROR: Invalid Vertex Usage Type");
		return;
	}

	glBindBuffer(buffType, VBO);
	glBufferData(buffType, numOfElements, bufferData, drawType);
	
}

int vertexArrayIndex = -1;
int vertexArrayStrideCount = 0;

void DreamGLGraphics::BindBuffer(BufferType type, size_t& VBO)
{
	unsigned int buffType = -1;

	switch (type) {
	case BufferType::VertexArray: {
		glBindVertexArray(VBO);
		return;
	}
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

void DreamGLGraphics::AddVertexAttributePointer(int size, unsigned int dataType, bool shouldNormalize, unsigned int sizeOf)
{
	vertexArrayIndex++;
	glEnableVertexAttribArray(vertexArrayIndex);
	//TODO: Take out GL_FLOAT
	glVertexAttribPointer(vertexArrayIndex, size, GL_FLOAT, shouldNormalize ? GL_FALSE:GL_TRUE, sizeOf, (void*)vertexArrayStrideCount);
	vertexArrayStrideCount += sizeOf;
}

void DreamGLGraphics::UnBindBuffer(BufferType type)
{
	switch (type) {
	case BufferType::VertexArray: {
		glBindVertexArray(0);
		vertexArrayIndex = -1;
		vertexArrayStrideCount = 0;
		return;
	}
	case BufferType::ArrayBuffer:
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		break;
	case BufferType::ElementArrayBuffer:
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		break;
	}
	
}

void DreamGLGraphics::TerminateGraphics()
{
	glfwTerminate();
}

void DreamGLGraphics::DestroyWindow()
{
	glfwDestroyWindow(window);
}

void DreamGLGraphics::InitGlad()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to Init GLAD");
	}
}

void DreamGLGraphics::DrawWithIndex(size_t size) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, (void*)0);
}

void DreamGLGraphics::DrawWithVertex(size_t size) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, size);
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
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//}
	//glBindVertexArray(obj->GetVertArr());
	
	//TODO: Need to figure out how to know when to draw using indices or not
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

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
		string fileContent;

		if (DreamFileIO::ReadFullFile(fileContent)) {

			switch (shaderType) {
			case ShaderType::VertexShader: {
				prog = glCreateShader(GL_VERTEX_SHADER);
				break;
			}
			case ShaderType::PixelShader: {
				prog = glCreateShader(GL_FRAGMENT_SHADER);
				break;
			}
			case ShaderType::GeometryShader: {
				prog = glCreateShader(GL_GEOMETRY_SHADER);
				break;
			}
			case ShaderType::ComputeShader: {
				//prog = glCreateShader(GL_COMPUTE_SHADER);
				break;
			}
			}
			
			if (prog != 0) {
				const char* content = fileContent.c_str();
				glShaderSource(prog, 1, (const GLchar**)&content, 0);

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
		return -1;
	}
}

void DreamGLGraphics::SetShader(unsigned int shaderProg)
{
	glUseProgram(shaderProg);
}
