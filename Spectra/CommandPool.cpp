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
			poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

			if (vkCreateCommandPool(device->getDevice(), &poolInfo, nullptr, commandPool.replace()) != VK_SUCCESS) {
				throw std::runtime_error("failed to create command pool!");
			}
		}

		VkCommandPool CommandPool::getPool() {
			return commandPool;
		}

		void CommandPool::clear() {
			vkResetCommandPool(Vulkan::getLogicalDevice()->getDevice(), commandPool, 0);
		}
	}
}