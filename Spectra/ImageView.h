#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VReference.h"

namespace spectra {
	namespace internal {
		class LogicalDevice;

		class ImageView {
		public:
			ImageView();
			~ImageView();

			void init(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
			VkImageView getImageView();
			bool isCreated();

		private:
			bool created = false;
			VReference<VkImageView> imageView;
		};
	}
}