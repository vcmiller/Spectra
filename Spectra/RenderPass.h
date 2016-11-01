#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VReference.h"
#include "CommandBuffer.h"
#include "Framebuffer.h"
#include "Color.h"

namespace spectra {
	namespace internal {
		class Window;

		class RenderPass {
		public:
			RenderPass();
			void init(Window *window, bool clearColor = true, bool clearDepth = true);
			VkRenderPass getRenderPass();

			void begin(CommandBuffer *commandBuffer, Framebuffer *framebuffer, int32_t posx, int32_t posy, uint32_t width, uint32_t height, Color clearColorValue = Color(0, 0, 0, 1));
			void end(CommandBuffer *commandBuffer);

		private:
			VReference<VkRenderPass> renderPass;
		};
	}
}