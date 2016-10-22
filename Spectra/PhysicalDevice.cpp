#include "PhysicalDevice.h"
#include "Log.h"
#include "Vulkan.h"
#include "Window.h"

#include <vector>
#include <set>

namespace spectra {
	namespace internal {


		List<const PhysicalDevice*> PhysicalDevice::enumerate() {
			if (enumerated) {
				return pointers;
			} else {
				uint32_t deviceCount = 0;
				vkEnumeratePhysicalDevices(Vulkan::getInstance(), &deviceCount, nullptr);

				if (deviceCount == 0) {
					throw std::runtime_error("No supported GPU detected");
				}

				std::vector<VkPhysicalDevice> devices(deviceCount);
				vkEnumeratePhysicalDevices(Vulkan::getInstance(), &deviceCount, devices.data());

				gpus = new PhysicalDevice[deviceCount];
				
				for (int i = 0; i < deviceCount; i++) {
					gpus[i] = PhysicalDevice(devices[i]);
					pointers.add(&gpus[i]);
				}

				enumerated = true;
				return pointers;
			}

		}

		bool PhysicalDevice::QueueFamilyIndices::isComplete() const {
			return graphicsFamily >= 0 && presentFamily >= 0;
		}

		PhysicalDevice::QueueFamilyIndices PhysicalDevice::getQueueFamilyIndices(Window * window) const {
			QueueFamilyIndices indices;

			uint32_t queueFamilyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

			std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

			int i = 0;
			for (const auto& queueFamily : queueFamilies) {
				if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
					indices.graphicsFamily = i;
				}

				VkBool32 presentSupport = false;
				vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, window->getSurface(), &presentSupport);

				if (queueFamily.queueCount > 0 && presentSupport) {
					indices.presentFamily = i;
				}

				if (indices.isComplete()) {
					break;
				}

				i++;
			}

			return indices;
		}

		PhysicalDevice::SwapChainSupportDetails PhysicalDevice::getSwapChainSupportDetails(Window *window) const {
			SwapChainSupportDetails details;

			auto surface = window->getSurface();

			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &details.capabilities);

			uint32_t formatCount;
			vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);

			if (formatCount != 0) {
				details.formats.resize(formatCount);
				vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, details.formats.data());
			}

			uint32_t presentModeCount;
			vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);

			if (presentModeCount != 0) {
				details.presentModes.resize(presentModeCount);
				vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, details.presentModes.data());
			}

			return details;
		}

		VkPhysicalDevice PhysicalDevice::getDevice() const {
			return physicalDevice;
		}

		VkPhysicalDeviceProperties PhysicalDevice::getProperties() const {
			return properties;
		}

		VkPhysicalDeviceFeatures PhysicalDevice::getFeatures() const {
			return features;
		}

		bool PhysicalDevice::checkExtensionSupport() const {
			uint32_t extensionCount;
			vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

			std::vector<VkExtensionProperties> availableExtensions(extensionCount);
			vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

			auto deviceExtensions = Vulkan::getDeviceExtensions();
			std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

			for (const auto& extension : availableExtensions) {
				requiredExtensions.erase(extension.extensionName);
			}

			return requiredExtensions.empty();
		}

		int PhysicalDevice::rate() const {
			int score = 0;

			// Discrete GPUs have a significant performance advantage
			if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
				score += 1000;
			}

			// Maximum possible size of textures affects graphics quality
			score += properties.limits.maxImageDimension2D;

			//auto indices = device->getQueueFamilyIndices();
			bool swapChainAdequate = false;
			bool extensionsSupported = checkExtensionSupport();

			/*
			if (extensionsSupported) {
			SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
			}

			// Application can't function without geometry shaders
			if (!(deviceFeatures.geometryShader && indices.isComplete() && extensionsSupported && swapChainAdequate)) {
			return 0;
			}*/

			if (!extensionsSupported) {
				return 0;
			}

			return score;
		}

		PhysicalDevice::PhysicalDevice(VkPhysicalDevice device) {
			physicalDevice = device;

			vkGetPhysicalDeviceProperties(device, &properties);
			vkGetPhysicalDeviceFeatures(device, &features);
		}

		PhysicalDevice::PhysicalDevice() {}

		PhysicalDevice::PhysicalDevice(const PhysicalDevice &other) {
			properties = other.properties;
			features = other.features;
			physicalDevice = other.physicalDevice;
		}

		void PhysicalDevice::operator=(const PhysicalDevice &other) {
			properties = other.properties;
			features = other.features;
			physicalDevice = other.physicalDevice;
		}

		List<const PhysicalDevice*> PhysicalDevice::pointers;
		PhysicalDevice *PhysicalDevice::gpus;
		bool PhysicalDevice::enumerated = false;

	}
}