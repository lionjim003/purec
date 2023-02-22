#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600
GLFWwindow* window;
VkInstance instance;
VkDebugUtilsMessengerEXT debugMessenger;
VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
struct {
	graphicsFaimly//<==== need c language understand.
} indices;
VkDevice device;
VkQueue graphicsQueue;

#ifdef NDEBUG
	bool const enableValidationLayers = false;
#else
	bool const enableValidationLayers = true;
#endif/* NDEBUG */

char const* const requiredExtensions[] = {
	VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
	NULL,
};
char const* const requiredLayers[] = {
	"VK_LAYER_KHRONOS_validation",
	NULL,	
};

void initWindow() {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Vulkan", NULL, NULL);
}
void printAvailableExtensions() {
	uint32_t count = 0;
	vkEnumerateInstanceExtensionProperties(NULL, &count, NULL);
	VkExtensionProperties extensions[count];
	vkEnumerateInstanceExtensionProperties(NULL, &count, extensions);
	
	printf("Available extensions:\n");
	for (uint32_t i = 0; i < count; ++i) {
		printf("%s\n", extensions[i].extensionName);
	}
}
uint32_t getRequiredExtensions(char const** pExtensions) {
	uint32_t count = 0;
	char const** glfwRequiredExtensions = 
		glfwGetRequiredInstanceExtensions(&count);
		
	uint32_t extensionCount = 0;
	for (uint32_t i = 0, already = false, ralready = false; true; ++i) {
		if (i < count) {
			if (pExtensions) pExtensions[i] = glfwRequiredExtensions[i];
			++extensionCount;
		} else already = true;
		
		if (enableValidationLayers && requiredExtensions[i] && !ralready) {
			if (pExtensions) pExtensions[count + i] = requiredExtensions[i];
			++extensionCount;
		} else ralready = true;
		
		if (already && ralready) 
			break;
	}
	return extensionCount;
}
void printAvailableLayers() {
	uint32_t count = 0;
	vkEnumerateInstanceLayerProperties(&count, NULL);
	VkLayerProperties layers[count];
	vkEnumerateInstanceLayerProperties(&count, layers);
	
	printf("Available layers:\n");
	for (uint32_t i = 0; i < count; ++i) {
		printf("%s\n", layers[i].layerName);
	}
}
uint32_t getRequiredLayers(char const** pLayers) {
	uint32_t layerCount = 0;
	for (uint32_t i = 0; requiredLayers[i]; ++i) {
		if (enableValidationLayers) {
			if (pLayers) pLayers[i] = requiredLayers[i];
			++layerCount;
		}
	}
	return layerCount;
}
VkResult createDebugUtilsMessengerEXT(VkInstance instance, 
	const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
	const VkAllocationCallbacks* pAllocator,
	VkDebugUtilsMessengerEXT* pDebugMessenger) {
	
	 PFN_vkCreateDebugUtilsMessengerEXT func =
	(PFN_vkCreateDebugUtilsMessengerEXT)
		vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		
	return (func) ? 
		func(instance, pCreateInfo, pAllocator, pDebugMessenger) :
		VK_ERROR_EXTENSION_NOT_PRESENT;
}
void destroyDebugUtilsMessengerEXT(VkInstance instance,
	VkDebugUtilsMessengerEXT debugMessenger,
	const VkAllocationCallbacks* pAllocator) {
	
	 PFN_vkDestroyDebugUtilsMessengerEXT func = 
	(PFN_vkDestroyDebugUtilsMessengerEXT)
		vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		
	if (func) func(instance, debugMessenger, pAllocator);
}
VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData) {
		
	if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
		printf("%s\n", pCallbackData->pMessage);
	return VK_FALSE;
}
void fillDebugUtilsMessengerCreateInfoEXT( 
	VkDebugUtilsMessengerCreateInfoEXT* createInfo) {

	createInfo->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo->messageSeverity =  
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo->messageType = 
		VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo->pfnUserCallback = debugCallback;
	createInfo->pUserData = NULL;
}
void initVulkan() {
	VkApplicationInfo appInfo = {
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationName 	= "Hello Triangle",
		.applicationVersion = VK_MAKE_VERSION(1, 0, 0),
		.pEngineName 		= "No Engine",
		.engineVersion 		= VK_MAKE_VERSION(1, 0, 0),
		.apiVersion 		= VK_API_VERSION_1_0,
	};
	uint32_t extensionCount = getRequiredExtensions(NULL);
	char const* extensions[extensionCount];
	getRequiredExtensions(extensions);
	// printAvailableExtensions();
	// for (uint32_t i = 0; i < extensionCount; ++i) {
		// printf("Extension: %s\n", extensions[i]);
	// }
	uint32_t layerCount = getRequiredLayers(NULL);
	char const* layers[layerCount];
	getRequiredLayers(layers);
	// printAvailableLayers();
	// for (uint32_t i = 0; i < layerCount; ++i) {
		// printf("Layer: %s\n", layers[i]);
	// }
	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
	fillDebugUtilsMessengerCreateInfoEXT(&debugCreateInfo);
	
	VkInstanceCreateInfo createInfo = {
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pApplicationInfo = &appInfo,
		.enabledExtensionCount = extensionCount,
		.ppEnabledExtensionNames = extensions,
		.enabledLayerCount = layerCount,
		.ppEnabledLayerNames = layers,
		.pNext = &debugCreateInfo,
	};
	if (!enableValidationLayers)
		createInfo.pNext = NULL;
	
	VkResult result = vkCreateInstance(&createInfo, NULL, &instance);
	if (result != VK_SUCCESS) 
		printf("failed to create Vulkan instance: %d\n", result);
}
void setupDebugMessenger() {
	if (!enableValidationLayers) return;	
	
	VkDebugUtilsMessengerCreateInfoEXT createInfo;
	fillDebugUtilsMessengerCreateInfoEXT (&createInfo);
	if (createDebugUtilsMessengerEXT(instance, &createInfo, NULL, &debugMessenger)
		!= VK_SUCCESS) printf("failed to set up debug messenger\n");
}
bool isDeviceSuitable(VkPhysicalDevice device) {
	// VkPhysicalDeviceProperties deviceProperties;
	// vkGetPhysicalDeviceProperties(device, &deviceProperties);
	// VkPhysicalDeviceFeatures deviceFeatures;
	// vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
	
	uint32_t familyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, NULL);
	VkQueueFamilyProperties families[familyCount];
	vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, families);
	
	for (uint32_t i = 0; i < familyCount; ++i) {
		if (families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFaimly = i;
		}
	}
	return true;
}
void pickPhysicalDevice() {
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, NULL);
	if (deviceCount == 0) {
		printf("failed to find GPUs with Vulkan support!");
		return;
	}
	VkPhysicalDevice devices[deviceCount];
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices);
	for (uint32_t i = 0; i < deviceCount; ++i) {
		if (isDeviceSuitable(devices[i])) {
			physicalDevice = devices[i];
			break;
		}
	}
	if (physicalDevice == VK_NULL_HANDLE) {
		printf("failed to find a suitable GPU!");
		return;
	}
}
void createLogicalDevice() {
	float queuePriority = 1.0f;
	VkDeviceQueueCreateInfo queueCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
		.queueFamilyIndex = indices.graphicsFaimly.value,
		.queueCount = 1,
		.pQueuePriorities = &queuePriority,
	};
	VkPhysicalDeviceFeatures deviceFeatures{};
	VkDeviceCreateInfo createInfo = {
		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.pQueueCreateInfos = &queueCreateInfo,
		.queueCreateInfoCount = 1,
		.pEnabledFeatures = &deviceFeatures,
		.enabledExtensionCount = 0,
	};
	/* for the compatible with older vulkan implementations */
	uint32_t extensionCount = getRequiredExtensions(NULL);
	createInfo.enabledExtensionCount = extensionCount;
	
	char const* extensions[extensionCount];
	getRequiredExtensions(extensions);
	createInfo.ppEnabledExtensionNames = extensions;
	
	if (vkCreateDevice(physicalDevice, &createInfo, NULL, &device)
		!= VK_SUCCESS) {
		printf("failed to create logical device!\n");
		return;
	}
	vkGetDeviceQueue(device, indices.graphicsFaimly.value, 0, &graphicsQueue);
}
void mainLoop() {
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}
}
void cleanup() {
	if (enableValidationLayers)
		destroyDebugUtilsMessengerEXT(instance, debugMessenger, NULL);
	vkDestroyInstance(instance, NULL);
	vkDestroyDevice(device, NULL);
	glfwDestroyWindow(window);
	glfwTerminate();
}
int main(int argc, char const* argv[]) {
	initWindow();
	initVulkan();
	setupDebugMessenger();
	pickPhysicalDevice();
	createLogicalDevice();
	mainLoop();
	cleanup();
	return EXIT_SUCCESS;
}