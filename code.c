#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600
GLFWwindow* window;
void initWindow() {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Vulkan", NULL, NULL);
}
#ifdef NDEBUG
	bool const enableValidationLayers = false;
#	else
	bool const enableValidationLayers = true;
#	endif

char const* const requiredExtensions[] = {
	VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
	NULL,
};
void printAvailableExtensions() {
	uint32_t count = 0;
	vkEnumerateInstanceExtensionProperties(NULL, &count, NULL);
	VkExtensionProperties extensions[count];
	vkEnumerateInstanceExtensionProperties(NULL, &count, extensions);
	
	printf("Available extensions:\n");
	for (uint32_t i = 0; i < count; ++i) {
		printf("%s\n", extensions[i].extensionName);
	}
	return;
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
char const* const requiredLayers[] = {
	"VK_LAYER_KHRONOS_validation",
	NULL,	
};
void printAvailableLayers() {
	uint32_t count = 0;
	vkEnumerateInstanceLayerProperties(&count, NULL);
	VkLayerProperties layers[count];
	vkEnumerateInstanceLayerProperties(&count, layers);
	
	printf("Available layers:\n");
	for (uint32_t i = 0; i < count; ++i) {
		printf("%s\n", layers[i].layerName);
	}
	return;
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
VkInstance instance;
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
	VkInstanceCreateInfo createInfo = {
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pApplicationInfo = &appInfo,
		.enabledExtensionCount = extensionCount,
		.ppEnabledExtensionNames = extensions,
		.enabledLayerCount = layerCount,
		.ppEnabledLayerNames = layers,
	};
	VkResult result = vkCreateInstance(&createInfo, NULL, &instance);
	if (result != VK_SUCCESS) {
		printf("failed to create Vulkan instance: %d\n", result);
	}
}
void mainLoop() {
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}
}
void cleanup() {	
	vkDestroyInstance(instance, NULL);
	glfwDestroyWindow(window);
	glfwTerminate();
}

int main(int argc, char const* argv[]) {
	initWindow();
	initVulkan();
	mainLoop();
	cleanup();
	return EXIT_SUCCESS;
}