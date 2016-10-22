#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VReference.h"
#include "List.h"

#include <vector>

namespace spectra {
	namespace internal {
		class Window;

		class PhysicalDevice {
		public:
			static List<const PhysicalDevice*> enumerate();

			struct QueueFamilyIndices {
				int graphicsFamily = -1;
				int presentFamily = -1;

				bool isComplete() const;
			};

			struct SwapChainSupportDetails {
				VkSurfaceCapabilitiesKHR capabilities;
				std::vector<VkSurfaceFormatKHR> formats;
				std::vector<VkPresentModeKHR> presentModes;
			};

			QueueFamilyIndices getQueueFamilyIndices(Window* window) const;
			SwapChainSupportDetails getSwapChainSupportDetails(Window* window) const;
			VkPhysicalDevice getDevice() const;
			VkPhysicalDeviceProperties getProperties() const;
			VkPhysicalDeviceFeatures getFeatures() const;

			bool checkExtensionSupport() const;
			int rate() const;

		private:

			PhysicalDevice(VkPhysicalDevice device);
			PhysicalDevice(const PhysicalDevice&);
			PhysicalDevice();
			void operator=(const PhysicalDevice&);

			VkPhysicalDeviceProperties properties;
			VkPhysicalDeviceFeatures features;
			VkPhysicalDevice physicalDevice;

			static List<const PhysicalDevice*> pointers;
			static PhysicalDevice *gpus;
			static bool enumerated;
		};
	}
}