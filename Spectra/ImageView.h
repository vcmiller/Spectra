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
			ImageView(VkImage image, LogicalDevice* device, VkFormat format, VkImageAspectFlags aspectFlags);

			void init(VkImage image, LogicalDevice* device, VkFormat format, VkImageAspectFlags aspectFlags);
			VkImageView getImageView();

		private:
			VkImageView imageView;
			VkDevice device;
		};
	}
}