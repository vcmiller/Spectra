#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VReference.h"
#include "CommandPool.h"

namespace spectra {
	namespace internal {
		class CommandBuffer {
		public:
			CommandBuffer();
			CommandBuffer(bool singleUse);
			CommandBuffer(CommandPool *commandPool, bool singleUse = true);

			~CommandBuffer();

			void init(bool singleUse = false);
			void init(CommandPool *commandPool, bool singleUse = false);

			void begin();
			void end();
			void submit();

			VkCommandBuffer getBuffer();

		private:
			bool singleUse;
			CommandPool *commandPool;
			VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
		};
	}
}