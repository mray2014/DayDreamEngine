#include "DreamGLGraphics.h"
#include <DreamFileIO.h>
#include <glad/glad.h>
#include <GLFW\glfw3.h>
#include <vector>
#include <iostream>

#include <SPIRV/spirv_glsl.hpp>
#pragma comment(lib, "spirv-cross-glsld.lib")
#pragma comment(lib, "spirv-cross-cored.lib")

//#include <glm\glm.hpp>
//#include <glm/gtx/transform.hpp>
//#include <glm/gtx/euler_angles.hpp>

#include <DreamTimeManager.h>
#include "DreamCameraManager.h"

GLFWwindow* window = nullptr;

void OnWindowResize(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void APIENTRY glDebugOutput(GLenum source,
	GLenum type,
	unsigned int id,
	GLenum severity,
	GLsizei length,
	const char* message,
	const void* userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	} std::cout << std::endl;
	std::cout << std::endl;
}


DreamGLGraphics::DreamGLGraphics() : DreamGraphics()
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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

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

	int flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);

		glDebugMessageControl(GL_DEBUG_SOURCE_API,
			GL_DEBUG_TYPE_ERROR,
			GL_DEBUG_SEVERITY_HIGH,
			0, nullptr, GL_TRUE);
	}

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

	DreamCameraManager* camManager = DreamCameraManager::GetInstance();
	matConstData.viewMat = camManager->GetCurrentCam_ViewMat();
	matConstData.projMat = camManager->GetCurrentCam_ProjMat();
	matConstData.totalTime = DreamTimeManager::totalTime;

	DreamBuffer* constDataBuffer = constDataBufferInfo.GetUniformBuffer(0);
	UpdateBufferData(constDataBuffer, &matConstData, sizeof(ConstantUniformData));
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, constDataBuffer->GetBufferPointer().GetStoredHandle());
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

	size_t handle = 0;

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
	case BufferType::UniformBuffer: {
		buffType = GL_UNIFORM_BUFFER;
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

	dataSize *= numOfElements;

	glBindBuffer(buffType, handle);
	glBufferData(buffType,  dataSize, bufferData, drawType);
	glBindBuffer(buffType, 0);

	return new DreamBuffer(handle, type, dataSize, numOfBuffers, &strides[0], &offests[0]);
}

DreamBuffer* DreamGLGraphics::GenerateBuffer(BufferType type, size_t bufferSize)
{
	return GenerateBuffer(type, nullptr, 1, { bufferSize }, {0}, StaticDraw);
}

void DreamGLGraphics::UpdateBufferData(DreamBuffer* buffer, void* bufferData, size_t bufSize, VertexDataUsage dataUsage) 
{
	size_t buffType = -1;
	size_t drawType = -1;
	size_t handle = buffer->GetBufferPointer().GetStoredHandle();

	switch (buffer->GetBufferType()) {
	case BufferType::ArrayBuffer: {
		buffType = GL_ARRAY_BUFFER;
		break;
	}
	case BufferType::ElementArrayBuffer: {
		buffType = GL_ELEMENT_ARRAY_BUFFER;
		break;
	}
	case BufferType::UniformBuffer: {
		buffType = GL_UNIFORM_BUFFER;
		break;
	}
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

	glBindBuffer(buffType, handle);
	glBufferData(buffType, bufSize, bufferData, drawType);
	glBindBuffer(buffType, 0);
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
	case BufferType::UniformBuffer:
		buffType = GL_UNIFORM_BUFFER;
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
	{
		/// ===============================================
		/// ======Using OpenGL to make a proper Proj Matrix
		/// ===============================================

		//float fovy = 3.14159f * 0.25f / zoom;
		//float aspect = DreamGraphics::GetAspectRatio();
		//float aspect = 1.0f;
		//float zNear = 0.01f;
		//float zFar = 1000.0f;

		//float aspect = DreamGraphics::GetAspectRatio();
		//float fovy = ((180.0f - camManager->currentCamera->fieldOfView) * DEG2RAD) / zoom;
		//float zNear = camManager->currentCamera->nearClipDist;
		//float zFar = camManager->currentCamera->farClipDist;

		//glm::mat4 proj = glm::perspective(fovy, aspect, zNear, zFar);
		//glUniformMatrix4fv(3, 1, GL_FALSE, &proj[0][0]);



		/// ===============================================
		/// ======Using OpenGL to make a proper View Matrix
		/// ===============================================
		//DreamVector3 pos = camManager->currentCamera->transform.position;
		//DreamVector3 forward = camManager->currentCamera->transform.GetForward();

		//glm::vec3 camPos = glm::vec3(pos.x, pos.y, pos.z);
		//glm::vec3 camForward = glm::vec3(forward.x, forward.y, forward.z);
		//glm::vec3 camCenter = camPos + camForward;
		//glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		//glm::mat4 view = glm::lookAt(camPos, camCenter, up);
		//glUniformMatrix4fv(2, 1, GL_FALSE, &view[0][0]);
		//glUniformMatrix4fv(2, 1, GL_FALSE, &view[0][0]);




		/// ===============================================
		/// ======Using OpenGL to bind uniform buffers to binding points
		/// ===============================================
		//DreamCameraManager* camManager = DreamCameraManager::GetInstance();
		//
		//DreamVector4 meshColor(1.0f, 1.0f, 1.0f, 1.0f);
		//float time = DreamTimeManager::totalTime;
		//
		//DreamMath::DreamTransform transform;
		//
		//GLuint vResult = glGetUniformBlockIndex(3, "ConstantData");
		//glUniformBlockBinding(3, vResult, UniformBufferLayout::ConstantData);
		//
		//
		//struct PixelexUBOData {
		//	DreamMath::DreamMatrix4X4 worldMat;
		//	DreamVector4 meshColor;
		//	float specular;
		//};
		//
		//PixelexUBOData pData;
		//pData.worldMat = transform.GetWorldMatrix();
		//pData.meshColor = DreamVector4(1.0f, 1.0f, 1.0f, 1.0f);
		//pData.specular = 1;
		//
		//size_t pHandle;
		//size_t pBuffSize = sizeof(PixelexUBOData);
		//glGenBuffers(1, (GLuint*)&pHandle);
		//glBindBuffer(GL_UNIFORM_BUFFER, pHandle);
		//glBufferData(GL_UNIFORM_BUFFER, pBuffSize, &pData, GL_STATIC_DRAW);
		//glBindBuffer(GL_UNIFORM_BUFFER, 0);
		//
		//glUniformBlockBinding(3, 0, 1);
		//glBindBufferRange(GL_UNIFORM_BUFFER, 1, pHandle, 0, pBuffSize);
	}

	//glDepthMask(GL_TRUE);
	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_BLEND);

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

DreamShader* DreamGLGraphics::LoadShader(const wchar_t* file, ShaderType shaderType) {
	using namespace std;

	bool hasMatUniform = false; 
	bool hasConstDataUniform = false;

	UniformList uniforms;
	UniformMembers uniformMembers;

	GLuint prog = -1;

	std::wstring wfile = L"";
	wfile.append(file);
	std::string convertFile(wfile.begin(), wfile.end());

	std::string path = "Shaders/";
	path.append(convertFile);
	path.append(".spv");

	DreamFileIO::OpenFileRead(path.c_str(), std::ios::ate | std::ios::binary);

	char* shaderCode = nullptr;
	size_t length;
	DreamFileIO::ReadFullFileQuick(&shaderCode, length);
	DreamFileIO::CloseFileRead();

	uint32_t* code = reinterpret_cast<uint32_t*>(shaderCode);

	// Read SPIR-V from disk or similar.
	std::vector<uint32_t> spirv_binary;
	spirv_cross::CompilerGLSL glsl(code, length / sizeof(uint32_t));

	// The SPIR-V is now parsed, and we can perform reflection on it.
	spirv_cross::ShaderResources resources = glsl.get_shader_resources();

	// Get all sampled images in the shader.
	for (auto& resource : resources.uniform_buffers)
	{
		std::string name = resource.name;

		//=======Grabbing uniform size and member data
		const spirv_cross::SPIRType type = glsl.get_type(resource.base_type_id); // What is the difference between base_type_ID and type_Id
		size_t structSize = glsl.get_declared_struct_size(type);

		int memberOffset = 0;
		for (int i = 0; i < type.member_types.size(); i++) {
			size_t memberSize = glsl.get_declared_struct_member_size(type, i);
			std::string memberName = glsl.get_member_name(resource.base_type_id, i);

			uniformMembers[memberName] = memberOffset;
			memberOffset += memberSize;
		}
		

		//=======Grabbing binding index of uniform
		unsigned set = glsl.get_decoration(resource.id, spv::DecorationDescriptorSet);
		unsigned binding = glsl.get_decoration(resource.id, spv::DecorationBinding);
		printf("Uniform Buffer %s at set = %u, binding = %u\n", resource.name.c_str(), set, binding);

		// Modify the decoration to prepare it for GLSL.
		glsl.unset_decoration(resource.id, spv::DecorationDescriptorSet);
		// Some arbitrary remapping if we want.
		glsl.set_decoration(resource.id, spv::DecorationBinding, set * 16 + binding);

		//=======Creating buffer for uniform
		if (name == "ConstantData") {
			hasConstDataUniform = true;
		}
		else if (name == "MaterialData") {
			hasMatUniform = true;
		}

		//=======Storing uniform
		if (hasConstDataUniform) {
			uniforms[name] = constDataBufferInfo;
		}
		else {
			uniforms[name] = UniformInfo(binding, structSize, uniformMembers);
		}
	}

	// Set some options.
	spirv_cross::CompilerGLSL::Options options;
	options.version = 450;
	options.es = false;
	glsl.set_common_options(options);

	// Compile to GLSL, ready to give to GL driver.
	std::string source = glsl.compile();

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
		const char* content = source.c_str();
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
			return new DreamShader(shaderType, DreamPointer(prog), uniforms, (hasMatUniform && hasConstDataUniform));
		}
	}
	
	return nullptr;
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
	unsigned int bindingPointIndex = 2;
	for (int i = 0; i < linkedShaders.size(); i++) {
		for (auto& uniformData : linkedShaders[i]->shaderUniforms) {
			std::string name = uniformData.first;

			if (name == "ConstantData") {
				bindingPoints[name] = 0;
			}
			else if (name == "MaterialData") {
				bindingPoints[name] = 1;
			}
			else {
				bindingPoints[name] = bindingPointIndex;
				bindingPointIndex++;
			}
		}
	}

	if (prog != 0) {
		glLinkProgram(prog);

		GLint result;
		glGetProgramiv(prog, GL_LINK_STATUS, &result);
		if (result == 0)
		{
			glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &result);
			GLchar debug[512];
			GLint size; //gives 0 when checked in debugger
			glGetProgramInfoLog(prog, 512, &size, debug);
			printf(debug);
			glDeleteProgram(prog);
			//delete[] debug; debug = nullptr;

		}
	}
	else {
		printf("ERROR: No Shader Program created!");
	}
}

void DreamGLShaderLinker::BindShaderLink(UniformIndexStore& indexStore)
{
	glUseProgram(prog);

	for (size_t i = 0; i < linkedShaders.size(); i++) {
		for (auto& uniformData : linkedShaders[i]->shaderUniforms) {
			uint32_t frameIndex = DreamGraphics::GetInstance()->currentFrame;
			uint32_t maxFramesInFlight = DreamGraphics::GetInstance()->GetMaxFramesInFlight();
			int index = (indexStore[uniformData.first] * maxFramesInFlight) + frameIndex;


			size_t handle = uniformData.second.buffers[index]->GetBufferPointer().GetStoredHandle();
			std::string name = uniformData.first;
			unsigned int bindPoint = bindingPoints[name];
			unsigned int bindIndex = uniformData.second.bindingIndex;

			glUniformBlockBinding(prog, bindIndex, bindPoint);

			if (name != "ConstantData") {
				glBindBufferBase(GL_UNIFORM_BUFFER, bindPoint, handle);
			}
		}
	}
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
