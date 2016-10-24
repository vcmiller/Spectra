#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VReference.h"

namespace spectra {
	namespace internal {
		class Window;
		class LogicalDevice;

		class CommandPool {
		public:
			CommandPool();

			void init(LogicalDevice *device, Window *window);
			VkCommandPool getPool();

		private:

			VReference<VkCommandPool> commandPool;
		};
	}
}