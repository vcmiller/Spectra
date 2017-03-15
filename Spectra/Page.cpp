#include "Page.h"
#include "Vulkan.h"
#include "LogicalDevice.h"

namespace spectra {
	namespace internal {

		VkDeviceMemory Page::getMemory() {
			return memory;
		}

		uint32_t Page::getSize() {
			return size;
		}

		void Page::free() {
			vkFreeMemory(Vulkan::getLogicalDevice()->getDevice(), memory, nullptr);
		}

		Page::Page(VkDeviceMemory memory, uint32_t size) {
			this->memory = memory;
			this->size = size;
		}

		Page::Page() {}

	}
}