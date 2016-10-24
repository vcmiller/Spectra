#include "CommandPool.h"
#include "LogicalDevice.h"
#include "Vulkan.h"
#include "Window.h"

namespace spectra {
	namespace internal {
		CommandPool::CommandPool() {}

		void CommandPool::init(LogicalDevice *device, Window *window) {
			commandPool = VReference<VkCommandPool>(device->getDevice(), vkDestroyCommandPool);

			auto queueFamilyIndices = device->getPhysicalDevice()->getQueueFamilyIndices(window);

			VkCommandPoolCreateInfo poolInfo = {};
			poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
			poolInfo.flags = 0; // Optional

			if (vkCreateCommandPool(device->getDevice(), &poolInfo, nullptr, commandPool.replace()) != VK_SUCCESS) {
				throw std::runtime_error("failed to create command pool!");
			}
		}
		VkCommandPool CommandPool::getPool() {
			return commandPool;
		}
	}
}