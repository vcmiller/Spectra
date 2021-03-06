#include "Vulkan.h"
#include "Log.h"
#include "Window.h"
#include "LogicalDevice.h"

const std::vector<const char*> validationLayers = {
	"VK_LAYER_LUNARG_standard_validation",
};

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

namespace spectra {
	namespace internal {

		bool Vulkan::enableValidation;

		void Vulkan::init(Config* config) {
			Vulkan::config = config;

			enableValidation = config->getBool("enable_validation", false);

			createInstance();
			setupDebugCallback();
			pickPhysicalDevice();
		}

		VkInstance Vulkan::getInstance() {
			return instance;
		}

		Config *Vulkan::getConfig() {
			return config;
		}

		void Vulkan::createInstance() {
			VkApplicationInfo appInfo;
			appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			appInfo.pApplicationName = "Spectra Application";
			appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.pEngineName = "Spectra";
			appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.apiVersion = VK_API_VERSION_1_0;

			VkInstanceCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			createInfo.pApplicationInfo = &appInfo;

			std::vector<const char*> extensions = getRequiredExtensions();

			createInfo.enabledExtensionCount = extensions.size();
			createInfo.ppEnabledExtensionNames = extensions.data();

			createInfo.enabledLayerCount = 0;

			if (enableValidation) {
				if (checkValidationLayerSupport(validationLayers)) {
					createInfo.enabledLayerCount = validationLayers.size();
					createInfo.ppEnabledLayerNames = validationLayers.data();
				} else {
					throw std::runtime_error("Validation layers not available");
				}
			}

			VkResult result = vkCreateInstance(&createInfo, nullptr, instance.replace());

			if (result != VK_SUCCESS) {
				throw std::runtime_error("Failed to create instance");
			}
		}

		void Vulkan::setupDebugCallback() {
			if (!enableValidation) return;

			callback = VReference<VkDebugReportCallbackEXT>(instance, DestroyDebugReportCallbackEXT);

			VkDebugReportCallbackCreateInfoEXT createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
			createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
			createInfo.pfnCallback = debugCallback;

			if (CreateDebugReportCallbackEXT(instance, &createInfo, nullptr, callback.replace()) != VK_SUCCESS) {
				throw std::runtime_error("failed to set up debug callback!");
			}
		}

		bool Vulkan::checkValidationLayerSupport(const std::vector<const char*> layers) {
			uint32_t layerCount;
			vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

			std::vector<VkLayerProperties> availableLayers(layerCount);
			vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

			for (const char* layerName : layers) {
				bool layerFound = false;

				for (const auto& layerProperties : availableLayers) {
					if (strcmp(layerName, layerProperties.layerName) == 0) {
						layerFound = true;
						break;
					}
				}

				if (!layerFound) {
					return false;
				}
			}

			return true;
		}

		std::vector<const char*> Vulkan::getRequiredExtensions() {
			std::vector<const char*> extensions;

			unsigned int glfwExtensionCount = 0;
			const char** glfwExtensions;
			glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

			for (unsigned int i = 0; i < glfwExtensionCount; i++) {
				extensions.push_back(glfwExtensions[i]);
			}

			if (enableValidation) {
				extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
			}

			return extensions;
		}

		void Vulkan::pickPhysicalDevice() {
			int bestScore = 0;
			Vulkan::physicalDevice = nullptr;

			for (auto device : PhysicalDevice::enumerate()) {
				int score = device->rate();
				if (score > bestScore) {
					bestScore = score;
					Vulkan::physicalDevice = device;
				}
			}

			if (!Vulkan::physicalDevice) {
				throw std::runtime_error("failed to find a suitable GPU!");
			}
		}

		void Vulkan::createLogicalDevice(Window *window) {
			logicalDevice.init(physicalDevice, window);
		}

		VKAPI_ATTR VkBool32 VKAPI_CALL Vulkan::debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char * layerPrefix, const char * msg, void * userData) {
			Log::log << "validation layer: " << msg << "\n";

			return VK_FALSE;
		}

		void Vulkan::windowErrorCallback(int error, const char * description) {
			Log::log << "GLFW Error: " << description << "\n";
		}

		void Vulkan::DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, VkAllocationCallbacks* pAllocator) {
			auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
			if (func != nullptr) {
				func(instance, callback, pAllocator);
			}
		}

		VkResult Vulkan::CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback) {
			auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
			if (func != nullptr) {
				return func(instance, pCreateInfo, pAllocator, pCallback);
			} else {
				return VK_ERROR_EXTENSION_NOT_PRESENT;
			}
		}

		Vulkan::Vulkan() { }

		const std::vector<const char*>& Vulkan::getValidationLayers() {
			return validationLayers;
		}

		const std::vector<const char*>& Vulkan::getDeviceExtensions() {
			return deviceExtensions;
		}

		bool Vulkan::getEnableValidationLayers() {
			return enableValidation;
		}

		const PhysicalDevice * Vulkan::getPhysicalDevice() {
			return physicalDevice;
		}

		LogicalDevice *Vulkan::getLogicalDevice() {
			return &logicalDevice;
		}

		VkFormat Vulkan::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
			for (VkFormat format : candidates) {
				VkFormatProperties props;
				vkGetPhysicalDeviceFormatProperties(physicalDevice->getDevice(), format, &props);

				if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
					return format;
				} else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
					return format;
				}

				throw std::runtime_error("failed to find supported format!");
			}
		}

		uint32_t Vulkan::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
			VkPhysicalDeviceMemoryProperties memProperties;
			vkGetPhysicalDeviceMemoryProperties(physicalDevice->getDevice(), &memProperties);

			for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
				if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
					return i;
				}
			}

			throw std::runtime_error("failed to find suitable memory type!");
		}


		const PhysicalDevice *Vulkan::physicalDevice = nullptr;
		Config *Vulkan::config;

		VReference<VkInstance> Vulkan::instance(vkDestroyInstance);
		VReference<VkDebugReportCallbackEXT> Vulkan::callback;
		LogicalDevice Vulkan::logicalDevice;

	}
}