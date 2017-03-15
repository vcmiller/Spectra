#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Page.h"
#include "Memory.h"
#include "List.h"
#include "Map.h"

namespace spectra {
	namespace internal {

		class MemoryManager {
		public:
			static Memory allocate(uint32_t size, uint32_t typeFilter, VkMemoryPropertyFlags properties);
			static Memory reallocate(Memory *memory, uint32_t size, uint32_t typeFilter, VkMemoryPropertyFlags properties);

		private:
			static Page* allocatePage(uint32_t size, uint32_t typeFilter, VkMemoryPropertyFlags properties);

			static Map<uint32_t, List<Page>> pageLists;
		};
	}
}