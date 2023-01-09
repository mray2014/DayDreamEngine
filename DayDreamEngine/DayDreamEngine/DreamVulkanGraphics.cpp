#include "DreamVulkanGraphics.h"

void DreamVulkanGraphics::OnWindowResize(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}


DreamVulkanGraphics::DreamVulkanGraphics()
{
}


DreamVulkanGraphics::~DreamVulkanGraphics()
{
}

long DreamVulkanGraphics::InitWindow(int w, int h, const char* title)
{
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

bool DreamVulkanGraphics::isDeviceSuitable(VkPhysicalDevice device) {

	/*VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);

	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);*/

	//QueueFamilyIndices indices = findQueueFamilies(device);

	return true;
}

DreamVulkanGraphics::QueueFamilyIndices DreamVulkanGraphics::findQueueFamilies(VkPhysicalDevice device) {
	QueueFamilyIndices indices;
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = i;
			break;
		}

		i++;
	}

	return indices;
}

long DreamVulkanGraphics::InitGraphics()
{

	// Creating instance
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "DayDreamEngine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	createInfo.enabledExtensionCount = glfwExtensionCount;
	createInfo.ppEnabledExtensionNames = glfwExtensions;
	createInfo.enabledLayerCount = 0;

	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
		printf("failed to create instance!");
		return -1;
	}



	// Finding physical device
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	if (deviceCount == 0) {
		printf("failed to find GPUs with Vulkan support!");
		return -1;
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	for (const auto& device : devices) {
		if (isDeviceSuitable(device)) {
			physicalDevice = device;
			break;
		}
	}

	if (physicalDevice == VK_NULL_HANDLE) {
		printf("failed to find a suitable GPU!");
		return -1;
	}



	// Creating logical device
	QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

	VkDeviceQueueCreateInfo queueCreateInfo{};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = indices.graphicsFamily;
	queueCreateInfo.queueCount = 1;

	float queuePriority = 1.0f;
	queueCreateInfo.pQueuePriorities = &queuePriority;

	VkPhysicalDeviceFeatures deviceFeatures{};

	VkDeviceCreateInfo deviceCreateInfo{};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
	deviceCreateInfo.queueCreateInfoCount = 1;

	deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

	deviceCreateInfo.enabledExtensionCount = 0;

	//if (enableValidationLayers) {
	//	deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
	//	deviceCreateInfo.ppEnabledLayerNames = validationLayers.data();
	//}
	//else {
	//	deviceCreateInfo.enabledLayerCount = 0;
	//}

	if (vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device) != VK_SUCCESS) {
		printf("failed to create logical device!");
		return -1;
	}

	vkGetDeviceQueue(device, indices.graphicsFamily, 0, &graphicsQueue);

	return 0;
}

void DreamVulkanGraphics::SetViewPort(int posX, int posY, int w, int h)
{
	width = w;
	height = h;
	glViewport(posX, posY, width, height);
}

bool DreamVulkanGraphics::CheckWindowClose()
{
	return glfwWindowShouldClose(window);
}

void DreamVulkanGraphics::ClearScreen()
{
}

void DreamVulkanGraphics::SwapBuffers()
{
}

void DreamVulkanGraphics::CheckInputs()
{
}

DreamVertexArray* DreamVulkanGraphics::GenerateVertexArray(DreamBuffer* vert, DreamBuffer* ind)
{
	return new DreamVulkanVertexArray(vert, ind);
}

DreamBuffer* DreamVulkanGraphics::GenerateBuffer(BufferType type, void* bufferData, size_t numOfElements, std::vector<size_t> strides, std::vector<size_t> offests, VertexDataUsage dataUsage)
{
	return nullptr;
}

void DreamVulkanGraphics::BindBuffer(BufferType type, DreamBuffer* buffer)
{
}

void DreamVulkanGraphics::BeginVertexLayout()
{
}

void DreamVulkanGraphics::AddVertexLayoutData(std::string dataName, int size, unsigned int dataType, bool shouldNormalize, unsigned int sizeOf)
{
}

DreamBuffer* DreamVulkanGraphics::FinalizeVertexLayout()
{
	return nullptr;
}

void DreamVulkanGraphics::UnBindBuffer(BufferType type)
{
}

bool DreamVulkanGraphics::LoadShader(const wchar_t* file, ShaderType shaderType, DreamPointer& ptr)
{
	return false;
}

void DreamVulkanGraphics::ReleaseShader(DreamShader* shader)
{
}

void DreamVulkanGraphics::DrawWithIndex(size_t size)
{
}

void DreamVulkanGraphics::DrawWithVertex(size_t size)
{
}

void DreamVulkanGraphics::Draw()
{
}

void DreamVulkanGraphics::TerminateGraphics()
{
	vkDestroyInstance(instance, nullptr);
	vkDestroyDevice(device, nullptr);
	glfwTerminate();
}

void DreamVulkanGraphics::DestroyWindow()
{
	glfwDestroyWindow(window);
}

void DreamVulkanGraphics::DestroyBuffer(DreamBuffer* buffer)
{
}

DreamVulkanShaderLinker::DreamVulkanShaderLinker()
{
}

void DreamVulkanShaderLinker::AttachShader(DreamShader* shader)
{
}

void DreamVulkanShaderLinker::Finalize()
{
}

void DreamVulkanShaderLinker::BindShaderLink()
{
}

void DreamVulkanShaderLinker::UnBindShaderLink()
{
}

DreamVulkanVertexArray::DreamVulkanVertexArray(DreamBuffer* vert, DreamBuffer* ind)
{
}

DreamVulkanVertexArray::~DreamVulkanVertexArray()
{
}

void DreamVulkanVertexArray::Bind()
{
}

void DreamVulkanVertexArray::UnBind()
{
}
