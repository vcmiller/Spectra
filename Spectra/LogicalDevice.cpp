#include "LogicalDevice.h"
#include "Vulkan.h"
#include "Window.h"
#include "Log.h"

namespace spectra {
	namespace internal {
		LogicalDevice::LogicalDevice() { }

		LogicalDevice::~LogicalDevice() {
		}

		void LogicalDevice::init(const PhysicalDevice* physicalDevice, Window* window) {
			this->physicalDevice = physicalDevice;

			PhysicalDevice::QueueFamilyIndices indices = physicalDevice->getQueueFamilyIndices(window);

			std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
			std::set<int> uniqueQueueFamilies = { indices.graphicsFamily, indices.presentFamily };

			float queuePriority = 1.0f;
			for (int queueFamily : uniqueQueueFamilies) {
				VkDeviceQueueCreateInfo queueCreateInfo = {};
				queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				queueCreateInfo.queueFamilyIndex = queueFamily;
				queueCreateInfo.queueCount = 1;
				queueCreateInfo.pQueuePriorities = &queuePriority;
				queueCreateInfos.push_back(queueCreateInfo);
			}

			VkPhysicalDeviceFeatures deviceFeatures = {};

			VkDeviceCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

			createInfo.pQueueCreateInfos = queueCreateInfos.data();
			createInfo.queueCreateInfoCount = (uint32_t)queueCreateInfos.size();

			createInfo.pEnabledFeatures = &deviceFeatures;

			auto extensions = Vulkan::getDeviceExtensions();

			createInfo.enabledExtensionCount = extensions.size();
			createInfo.ppEnabledExtensionNames = extensions.data();

			auto layers = Vulkan::getValidationLayers();

			if (Vulkan::getEnableValidationLayers()) {

				createInfo.enabledLayerCount = layers.size();
				createInfo.ppEnabledLayerNames = layers.data();
			} else {
				createInfo.enabledLayerCount = 0;
			}

			if (vkCreateDevice(physicalDevice->getDevice(), &createInfo, nullptr, device.replace()) != VK_SUCCESS) {
				throw std::runtime_error("Failed to create logical device!");
			}

			vkGetDeviceQueue(device, indices.graphicsFamily, 0, &graphicsQueue);
			vkGetDeviceQueue(device, indices.presentFamily, 0, &presentQueue);
		}

		VkDevice LogicalDevice::getDevice() {
			return device;
		}
	}
}