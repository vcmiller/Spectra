#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VReference.h"
#include "Image.h"

namespace spectra {
	namespace internal {
		class RenderPass;
		class Framebuffer {
		public:
			Framebuffer();

			void init(int width, int height, RenderPass *renderPass, ImageView* color, ImageView *depth = nullptr);

			VkFramebuffer getBuffer();

			int getWidth();
			int getHeight();

		private:
			int width;
			int height;

			VReference<VkFramebuffer> buffer;
		};
	}
}