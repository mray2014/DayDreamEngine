#pragma once
#include "DreamGraphics.h"

#ifdef DREAM_VULKAN
#define GLFW_INCLUDE_VULKAN
#endif

#include <GLFW/glfw3.h>


class DreamVulkanShaderLinker : public DreamShaderLinker {
protected:
	DreamVulkanShaderLinker();
public:
	void AttachShader(DreamShader* shader) override;
	void Finalize() override;
	void BindShaderLink() override;
	void UnBindShaderLink() override;

private:
	size_t prog;
	friend class DreamGraphics;
};

class DreamVulkanVertexArray : public DreamVertexArray {
public:
	DreamVulkanVertexArray(DreamBuffer* vert, DreamBuffer* ind = nullptr);
	~DreamVulkanVertexArray();

	void Bind() override;
	void UnBind() override;

	DreamBuffer* VAO;
};


class DreamVulkanGraphics : public DreamGraphics
{
public:
	DreamVulkanGraphics();
	~DreamVulkanGraphics();

	long InitWindow(int w, int h, const char* title) override;
	long InitGraphics() override;
	void SetViewPort(int posX, int posY, int w, int h) override;
	bool CheckWindowClose() override;

	void ClearScreen() override;
	void SwapBuffers() override;
	void CheckInputs() override;
	DreamVertexArray* GenerateVertexArray(DreamBuffer* vert, DreamBuffer* ind = nullptr);
	DreamBuffer* GenerateBuffer(BufferType type, void* bufferData = nullptr, size_t numOfElements = 0, std::vector<size_t> strides = { 0 }, std::vector<size_t> offests = { 0 }, VertexDataUsage dataUsage = VertexDataUsage::StaticDraw) override;
	void BindBuffer(BufferType type, DreamBuffer* buffer) override;
	void BeginVertexLayout() override;
	void AddVertexLayoutData(std::string dataName, int size, unsigned int dataType, bool shouldNormalize, unsigned int sizeOf) override;
	DreamBuffer* FinalizeVertexLayout() override;
	void UnBindBuffer(BufferType type) override;
	bool LoadShader(const wchar_t* file, ShaderType shaderType, DreamPointer& ptr) override;
	void ReleaseShader(DreamShader* shader) override;
	void DrawWithIndex(size_t size) override;
	void DrawWithVertex(size_t size) override;
	void Draw() override;

	void TerminateGraphics() override;
	void DestroyWindow() override;
	void DestroyBuffer(DreamBuffer* buffer) override;

	bool isDeviceSuitable(VkPhysicalDevice device);
	static void OnWindowResize(GLFWwindow* window, int width, int height);

	struct QueueFamilyIndices {
		uint32_t graphicsFamily;
	};

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

private:
	VkInstance instance;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device;
	VkQueue graphicsQueue;
	GLFWwindow* window = nullptr;
};

