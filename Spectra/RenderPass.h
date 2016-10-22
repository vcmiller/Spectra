#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VReference.h"
#include "Window.h"

namespace spectra {
	namespace internal {
		class RenderPass {
		public:
			RenderPass();
			void init(Window *window);
			VkRenderPass getRenderPass();

		private:
			VReference<VkRenderPass> renderPass;
		};
	}
}