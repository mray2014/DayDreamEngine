#pragma once
#include "DreamGraphics.h"
#include <SPIRV/spirv_cross.hpp>
#pragma comment(lib, "spirv-cross-reflectd.lib")
#pragma comment(lib, "spirv-cross-cored.lib")
#define VK_USE_PLATFORM_WIN32_KHR

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>


#include <optional>

class DreamVulkanGraphics;

class DreamVulkanShaderLinker : public DreamShaderLinker {
protected:
	DreamVulkanShaderLinker();
public:
	~DreamVulkanShaderLinker() override;
	void AttachShader(DreamShader* shader) override;
	void Finalize() override;
	void BindShaderLink(UniformIndexStore& indexStore) override;
	void UnBindShaderLink() override;

private:
	DreamVulkanGraphics* vulkanGraphics = nullptr;
	std::vector<VkPipelineShaderStageCreateInfo> shadersStageInfo;
	std::vector<VkDescriptorSetLayoutBinding> descriptorBindings;
	std::vector <VkDescriptorSet> pipelineDescSet;
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;
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

struct VulkanBufferContainer {
	VkBuffer buffer;
	VkDeviceMemory bufferMemory;

	VulkanBufferContainer(VkBuffer buf, VkDeviceMemory bufMem) {
		buffer = buf;
		bufferMemory = bufMem;
	}
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
	DreamBuffer* GenerateBuffer(BufferType type, size_t bufferSize = 0) override;
	void UpdateBufferData(DreamBuffer* buffer, void* bufferData = nullptr, size_t bufSize = 0, VertexDataUsage dataUsage = VertexDataUsage::StaticDraw) override;
	void BindBuffer(BufferType type, DreamBuffer* buffer) override;
	void BeginVertexLayout() override;
	void AddVertexLayoutData(std::string dataName, int size, unsigned int location, bool shouldNormalize, unsigned int sizeOf) override;
	DreamBuffer* FinalizeVertexLayout() override;
	void UnBindBuffer(BufferType type) override;
	DreamShader* LoadShader(const wchar_t* file, ShaderType shaderType) override;
	void ReleaseShader(DreamShader* shader) override;
	void DrawWithIndex(size_t size) override;
	void DrawWithVertex(size_t size) override;
	void Draw() override;

	void TerminateGraphics() override;
	void DestroyWindow() override;
	void DestroyBuffer(DreamBuffer* buffer) override;

	bool isDeviceSuitable(VkPhysicalDevice device);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	static void OnWindowResize(GLFWwindow* window, int width, int height);

	void createInstance();
	bool checkValidationLayerSupport();
	std::vector<const char*> getRequiredExtensions();
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	void setupDebugMessenger();
	void createSurface();
	void pickPhysicalDevice();
	void createLogicalDevice();
	void createSwapChain();

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};


	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	void createImageViews();
	void createRenderPass();

	VkPipeline CreateGraphicPipeLine(std::vector<VkPipelineShaderStageCreateInfo>& shadersStageInfo, VkPipelineLayout& layout, std::vector<VkDescriptorSet>& pipelineDescSet, std::vector<VkDescriptorSetLayoutBinding>& descriptorBindings);

	std::vector<VkFramebuffer> swapChainFramebuffers;
	void createFramebuffers();
	void createCommandPool();
	void createCommandBuffer();
	void createSyncObjects();
	void recordCommandBuffer(VkCommandBuffer commandBuffer, size_t imageIndex);
	void BindGraphicsPipeline(VkPipeline pipeline);
	void DestroyGraphicsPipeline(VkPipeline pipe, VkPipelineLayout layout);
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	void recreateSwapChain();
	void cleanupSwapChain();

	void createDescriptorPool();
	void updateDescriptorSet(VkWriteDescriptorSet& descSet);
	void BindDescriptorSet(VkDescriptorSet descSet, VkPipelineLayout layout);

private:
	uint32_t imageIndex;
	bool framebufferResized = false;

	const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
	};

	std::vector<VkVertexInputAttributeDescription> attributeDesc;
#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif

	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	VkSwapchainKHR swapChain;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

	VkSurfaceKHR surface;

	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;

	std::vector<VkImage> swapChainImages;
	std::vector<VkImageView> swapChainImageViews;

	VkRenderPass renderPass;
	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;

	std::vector<VkDescriptorSetLayout> descSetLayouts;

	GLFWwindow* window = nullptr;
};

