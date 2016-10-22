#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VReference.h"
#include "Config.h"
#include "PhysicalDevice.h"

#include <vector>

namespace spectra {
	namespace internal {
		class Vulkan {
		public:
			static void init(Config *config);
			static VkInstance getInstance();
			static Config* getConfig();

			static const std::vector<const char*> &getValidationLayers();
			static const std::vector<const char*> &getDeviceExtensions();
			static bool getEnableValidationLayers();

			static const PhysicalDevice *getPhysicalDevice();

		private:
			static void createInstance();
			static void setupDebugCallback(); 
			static void pickPhysicalDevice();
			static bool checkValidationLayerSupport(const std::vector<const char*> layers);
			static std::vector<const char*> getRequiredExtensions();

			static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char * layerPrefix, const char * msg, void * userData);
			static void windowErrorCallback(int error, const char* description);

			static void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, VkAllocationCallbacks* pAllocator);
			static VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);
			
			Vulkan();

			static VReference<VkInstance> instance;
			static VReference<VkDebugReportCallbackEXT> callback;

			static const PhysicalDevice *physicalDevice;

			static Config* config;
		};
	}
}