#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VReference.h"
#include "List.h"

namespace spectra {
	namespace internal {
		class Buffer {
		public:
			Buffer();
			void init(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);

			template <typename T> void init(List<T> list, VkBufferUsageFlags usage, bool visible = false) {
				VkDeviceSize bufferSize = sizeof(list[0]) * list.length();
				if (visible) {
					init(bufferSize, usage, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
				
					void* data;
					auto device = internal::Vulkan::getLogicalDevice();

					vkMapMemory(device->getDevice(), memory, 0, bufferSize, 0, &data);
					memcpy(data, list.begin(), (size_t)bufferSize);
					vkUnmapMemory(device->getDevice(), memory);
				} else {

					internal::Buffer stagingBuffer;
					stagingBuffer.init(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

					void* data;
					auto device = internal::Vulkan::getLogicalDevice();

					vkMapMemory(device->getDevice(), stagingBuffer.getMemory(), 0, bufferSize, 0, &data);
					memcpy(data, list.begin(), (size_t)bufferSize);
					vkUnmapMemory(device->getDevice(), stagingBuffer.getMemory());

					init(bufferSize, usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

					internal::Buffer::copy(&stagingBuffer, this);
				}
			}

			VkBuffer getBuffer();
			VkDeviceMemory getMemory();

			static void copy(Buffer* src, Buffer* dest);

			VkDeviceSize getSize();

		private:
			VkDeviceSize size;

			VReference<VkBuffer> buffer;
			VReference<VkDeviceMemory> memory;
		};
	}
}