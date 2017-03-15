#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace spectra {
	namespace internal {
		class Page;
		class Memory {
		public:
			Page *getPage();
			uint32_t getSize();
			uint32_t getOffset();
			
			void release();

		private:
			Page *page;
			uint32_t size;
			uint32_t offset;
		};
	}
}