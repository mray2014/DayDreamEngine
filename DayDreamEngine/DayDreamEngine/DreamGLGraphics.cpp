#include "DreamGLGraphics.h"
#include <DreamFileIO.h>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <vector>

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

DreamVertexArray* DreamGLGraphics::GenerateVertexArray(DreamBuffer* vert, DreamBuffer* ind)
{
	return  new DreamGLVertexArray(vert, ind);
}

DreamBuffer* DreamGLGraphics::GenerateBuffer(BufferType type, void* bufferData, size_t numOfElements, std::vector<size_t> strides, std::vector<size_t> offests, VertexDataUsage dataUsage)
{
	size_t buffType = -1;
	size_t drawType = -1;

	size_t numOfBuffers = strides.size();

	size_t handle;

	glGenBuffers(numOfBuffers, (GLuint*)&handle);

	switch (type) {
	case BufferType::ArrayBuffer: {
		buffType = GL_ARRAY_BUFFER;
		break;
	}
	case BufferType::ElementArrayBuffer: {
		buffType = GL_ELEMENT_ARRAY_BUFFER;
		break;
	}
	}

	if (buffType == -1) {
		printf("ERROR: Invalid Buffer Type");
		return nullptr;
	}


	if (numOfBuffers <= 0) {
		printf("ERROR: 0 Buffer strides input");
		return nullptr;
	}
	if (numOfBuffers != offests.size()) {
		printf("ERROR: Stride and offset buffer do not match!");
		return nullptr;
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
		return nullptr;
	}

	size_t dataSize = 0;

	for (int i = 0; i < numOfBuffers; i++) {
		dataSize += strides[i];
	}

	glBindBuffer(buffType, handle);
	glBufferData(buffType, numOfElements * dataSize, bufferData, drawType);
	glBindBuffer(buffType, 0);

	return new DreamBuffer(handle, numOfBuffers, &strides[0], &offests[0]);
}

void DreamGLGraphics::BindBuffer(BufferType type, DreamBuffer* buffer)
{
	unsigned int buffType = -1;

	size_t storedHandle = buffer->GetBufferPointer().GetStoredHandle();

	switch (type) {
	case BufferType::ArrayBuffer:
		buffType = GL_ARRAY_BUFFER;
		break;
	case BufferType::ElementArrayBuffer:
		buffType = GL_ELEMENT_ARRAY_BUFFER;
		break;
	}

	if (buffType != -1) {
		glBindBuffer(buffType, storedHandle);
	}
	else {
		printf("ERROR: Invalid Buffer Type");
	}
	
}

size_t vertLayoutHandle = -1;

size_t vertexArrayIndex = -1;
size_t vertexArrayStrideCount = 0;

void DreamGLGraphics::BeginVertexLayout()
{
	if (vertLayoutHandle == -1)
	{
		glGenVertexArrays(1, (GLuint*)&vertLayoutHandle);

		glBindVertexArray(vertLayoutHandle);
	}
	else {
		printf("ERROR: Vertex Layout creation process has started already!\nCall FinalizeVertexLayout to end the current operation and start a new one");
	}
	
}

void DreamGLGraphics::AddVertexLayoutData(std::string dataName, int size, unsigned int dataType, bool shouldNormalize, unsigned int sizeOf)
{
	if (vertLayoutHandle != -1) {
		vertexArrayIndex++;
		glEnableVertexAttribArray(vertexArrayIndex);
		//TODO: Take out GL_FLOAT
		glVertexAttribPointer(vertexArrayIndex, size, GL_FLOAT, shouldNormalize ? GL_FALSE : GL_TRUE, sizeOf, (void*)vertexArrayStrideCount);
		vertexArrayStrideCount += sizeOf;
	}
	else {
		printf("ERROR: No Vertex Layout creation process has started! Can't Add Data");
	}
}

DreamBuffer* DreamGLGraphics::FinalizeVertexLayout()
{
	if (vertLayoutHandle != -1)
	{
		glBindVertexArray(0);
		DreamBuffer* buff = new DreamBuffer(vertLayoutHandle);


		vertLayoutHandle = -1;
		vertexArrayIndex = -1;
		vertexArrayStrideCount = 0;

		return buff;
	}
	else {
		printf("ERROR: No Vertex Layout creation process has started! Can't Finalize");
		return nullptr;
	}
}

void DreamGLGraphics::UnBindBuffer(BufferType type)
{
	switch (type) {
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



bool DreamGLGraphics::LoadShader(const wchar_t* file, ShaderType shaderType, DreamPointer& ptr) {
	using namespace std;

	bool success = false;
	GLuint prog = -1;

	std::wstring wfile = L"";
	wfile.append(file);
	std::string convertFile(wfile.begin(), wfile.end());

	std::string path = "Shaders/";
	path.append(convertFile);
	path.append(".glsl");

	if (DreamFileIO::OpenFileRead(path.c_str()))
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
			
			if (prog != -1) {
				std::string str(fileContent.begin(), fileContent.end());
				const char* content = str.c_str();
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
				else {
					ptr = DreamPointer(prog);
					success = true;
				}
			}
		}
		DreamFileIO::CloseFileRead();
	}

	
	return success;
}

void DreamGLGraphics::ReleaseShader(DreamShader* shader)
{
}

void DreamGLGraphics::DestroyBuffer(DreamBuffer* buffer)
{
	if (buffer) {
		glDeleteBuffers(1, (GLuint*)&buffer->GetBufferPointer().GetStoredHandle());

		delete buffer;
		buffer = nullptr;
	}
}

DreamGLShaderLinker::DreamGLShaderLinker()
{
	if (prog == 0) {
		prog = glCreateProgram();
	}
	else {
		printf("ERROR: Shader Program creation in progress!");
	}
}

void DreamGLShaderLinker::AttachShader(DreamShader* shader)
{
	linkedShaders.push_back(shader);

	if (prog != 0) {
		glAttachShader(prog, shader->GetShaderPtr().GetStoredHandle());
	}
	else {
		printf("ERROR: No Shader created!");
	}
}

void DreamGLShaderLinker::Finalize()
{
	if (prog != 0) {
		glLinkProgram(prog);

		GLint result;
		glGetProgramiv(prog, GL_LINK_STATUS, &result);
		if (result == 0)
		{
			glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &result);
			GLchar* debug = new GLchar[result];

			glGetProgramInfoLog(prog, result, 0, debug);
			printf(debug);
			glDeleteProgram(prog);
			delete[] debug; debug = nullptr;

		}
	}
	else {
		printf("ERROR: No Shader Program created!");
	}
}

void DreamGLShaderLinker::BindShaderLink()
{
	for (size_t i = 0; i < linkedShaders.size(); i++) {
		linkedShaders[i]->BindShaderData();
	}
	
	glUseProgram(prog);
}

void DreamGLShaderLinker::UnBindShaderLink()
{
	glUseProgram(0);
}



DreamGLVertexArray::DreamGLVertexArray(DreamBuffer* vert, DreamBuffer* ind) : DreamVertexArray(vert, ind)
{
	graphics->BindBuffer(ArrayBuffer, vertexBuffer);
	graphics->BeginVertexLayout();
	graphics->AddVertexLayoutData("POSITION", 3, 0, false, sizeof(DreamVector3));
	VAO = graphics->FinalizeVertexLayout();
}

DreamGLVertexArray::~DreamGLVertexArray()
{
	DreamVertexArray::~DreamVertexArray();
	graphics->DestroyBuffer(VAO);
}

void DreamGLVertexArray::Bind()
{
	glBindVertexArray(VAO->GetBufferPointer().GetStoredHandle());
	if (indexBuffer) {
		graphics->BindBuffer(BufferType::ElementArrayBuffer, indexBuffer);
	}
}

void DreamGLVertexArray::UnBind()
{
	glBindVertexArray(0);
	graphics->UnBindBuffer(BufferType::ElementArrayBuffer);
}
