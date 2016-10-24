#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VReference.h"
#include "CommandBuffer.h"
#include "Framebuffer.h"

namespace spectra {
	namespace internal {
		class Window;

		class RenderPass {
		public:
			RenderPass();
			void init(Window *window);
			VkRenderPass getRenderPass();

			void begin(CommandBuffer *commandBuffer, Framebuffer *framebuffer);
			void end(CommandBuffer *commandBuffer);

		private:
			VReference<VkRenderPass> renderPass;
		};
	}
}