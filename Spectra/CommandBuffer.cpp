#include "CommandBuffer.h"
#include "Vulkan.h"
#include "LogicalDevice.h"

namespace spectra {
	namespace internal {
		CommandBuffer::CommandBuffer() {}

		CommandBuffer::CommandBuffer(bool singleUse) {
			init(singleUse);
		}

		CommandBuffer::CommandBuffer(CommandPool *commandPool, bool singleUse) {
			init(commandPool, singleUse);
		}

		CommandBuffer::~CommandBuffer() {
			vkFreeCommandBuffers(Vulkan::getLogicalDevice()->getDevice(), commandPool->getPool(), 1, &commandBuffer);
		}

		void CommandBuffer::init(bool singleUse) {
			init(Vulkan::getLogicalDevice()->getCommandPool());
		}

		void CommandBuffer::init(CommandPool *commandPool, bool singleUse) {
			this->commandPool = commandPool;
			this->singleUse = singleUse;

			if (commandBuffer) {
				vkFreeCommandBuffers(Vulkan::getLogicalDevice()->getDevice(), commandPool->getPool(), 1, &commandBuffer);
			}

			VkCommandBufferAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			allocInfo.commandPool = commandPool->getPool();
			allocInfo.commandBufferCount = 1;

			vkAllocateCommandBuffers(Vulkan::getLogicalDevice()->getDevice(), &allocInfo, &commandBuffer);
		}

		void CommandBuffer::begin() {
			VkCommandBufferBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = singleUse ? VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT : VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

			vkBeginCommandBuffer(commandBuffer, &beginInfo);
		}

		void CommandBuffer::end() {
			vkEndCommandBuffer(commandBuffer);
		}

		void CommandBuffer::submit() {
			VkSubmitInfo submitInfo = {};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &commandBuffer;

			VkQueue graphicsQueue = Vulkan::getLogicalDevice()->getGraphicsQueue();

			vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
			vkQueueWaitIdle(graphicsQueue);
		}

		VkCommandBuffer CommandBuffer::getBuffer() {
			return commandBuffer;
		}
	}
}
