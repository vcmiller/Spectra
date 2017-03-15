#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VReference.h"

namespace spectra {
	namespace internal {

		const int DefaultPageSize = 1000000;

		class MemoryManager;

		class Page {
		public:
			VkDeviceMemory getMemory();
			uint32_t getSize();

		private:
			friend class MemoryManager;

			VkDeviceMemory memory;
			uint32_t size;

			void free();

			Page(VkDeviceMemory memory, uint32_t size);
			Page();
		};
	}
}