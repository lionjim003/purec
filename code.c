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
char const* const validationLayers[] = {
	"VK_LAYER_KHRONOS_validation",
	NULL,
};
char const* const* supportValidationLayers(uint32_t* pLayerCount) {
	uint32_t layerCount = 0;
	vkEnumerateInstanceLayerProperties(&layerCount, NULL);
	printf("layer count: %u\n", layerCount);
	VkLayerProperties layers[layerCount];
	vkEnumerateInstanceLayerProperties(&layerCount, layers);
	
	
	bool validationLayersAllSupported = false;
	uint32_t vLayerCount = 0;
	for (uint32_t i = 0; i < layerCount; ++i) {
		printf("layer name:\t%s\n", layers[i].layerName);
		if (strcmp(validationLayers[vLayerCount], layers[i].layerName) == 0) {
			if (validationLayers[++vLayerCount] == NULL) {
				validationLayersAllSupported = true;
				break;
			}
			i = ~0;
		}
	}
	*pLayerCount = validationLayersAllSupported? vLayerCount: 0;
	return validationLayersAllSupported? validationLayers: NULL;
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
	uint32_t extensionCount = 0;
	uint32_t layerCount 	= 0;
	char const** extensions = glfwGetRequiredInstanceExtensions(&extensionCount);
	char const*const* layers= supportValidationLayers(&layerCount);
	if (!enableValidationLayers || layerCount == 0) {
		printf("not support validation layers.\n");
		layerCount = 0;
		layers = NULL;
	}
	VkInstanceCreateInfo createInfo = {
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pApplicationInfo = &appInfo,
		.enabledExtensionCount = extensionCount,
		.ppEnabledExtensionNames = extensions,
		.enabledLayerCount = layerCount,
		.ppEnabledLayerNames = layers,
	};
	if (vkCreateInstance(&createInfo, NULL, &instance) != VK_SUCCESS) {
		printf("failed to create Vulkan instance.\n");
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