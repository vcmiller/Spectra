#include "Buffer.h"
#include "Vulkan.h"
#include "LogicalDevice.h"
#include "CommandBuffer.h"

namespace spectra {
	namespace internal {
		Buffer::Buffer() {}

		void Buffer::init(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties) {
			this->size = size;

			LogicalDevice *device = Vulkan::getLogicalDevice();

			buffer.cleanup();
			memory.cleanup();

			buffer = VReference<VkBuffer>(device->getDevice(), vkDestroyBuffer);
			memory = VReference<VkDeviceMemory>(device->getDevice(), vkFreeMemory);

			VkBufferCreateInfo bufferInfo = {};
			bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferInfo.size = size;
			bufferInfo.usage = usage;
			bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

			if (vkCreateBuffer(device->getDevice(), &bufferInfo, nullptr, buffer.replace()) != VK_SUCCESS) {
				throw std::runtime_error("failed to create buffer!");
			}

			VkMemoryRequirements memRequirements;
			vkGetBufferMemoryRequirements(device->getDevice(), buffer, &memRequirements);

			VkMemoryAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocInfo.allocationSize = memRequirements.size;
			allocInfo.memoryTypeIndex = Vulkan::findMemoryType(memRequirements.memoryTypeBits, properties);

			if (vkAllocateMemory(device->getDevice(), &allocInfo, nullptr, memory.replace()) != VK_SUCCESS) {
				throw std::runtime_error("failed to allocate buffer memory!");
			}

			vkBindBufferMemory(device->getDevice(), buffer, memory, 0);
		}

		VkBuffer Buffer::getBuffer() {
			return buffer;
		}

		VkDeviceMemory Buffer::getMemory() {
			return memory;
		}

		void Buffer::copy(Buffer *src, Buffer *dest) {
			CommandBuffer cmd(true);
			cmd.begin();

			VkBufferCopy copyRegion = {};
			copyRegion.size = src->size;
			vkCmdCopyBuffer(cmd.getBuffer(), src->getBuffer(), dest->getBuffer(), 1, &copyRegion);

			cmd.end();
			cmd.submit();
		}

		VkDeviceSize Buffer::getSize() {
			return size;
		}
	}
}