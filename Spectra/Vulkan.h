#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VReference.h"

#include <vector>

namespace spectra {
	namespace internal {
		class Vulkan {
		public:
			static void init();
			static VkInstance getInstance();

		private:
			static void createInstance();
			static void setupDebugCallback(); 
			static bool checkValidationLayerSupport(const std::vector<const char*> layers);
			static std::vector<const char*> getRequiredExtensions();

			static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char * layerPrefix, const char * msg, void * userData);

			static void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, VkAllocationCallbacks* pAllocator);
			static VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);
			
			Vulkan();

			static VReference<VkInstance> instance;
			static VReference<VkDebugReportCallbackEXT> callback;
		};
	}
}