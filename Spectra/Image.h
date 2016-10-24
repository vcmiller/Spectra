#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VReference.h"
#include "ImageView.h"

namespace spectra {
	namespace internal {

		class Image {
		public:
			Image();
			void init(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkSampleCountFlagBits sampleCount);

			ImageView* getImageView();
			void createImageView(VkImageAspectFlags aspectFlags);

			void transition(VkImageLayout oldLayout, VkImageLayout newLayout);

			static bool hasStencil(VkFormat format);

			VkImage getImage();
			VkDeviceMemory getMemory();

			static void copy(Image* src, Image* dest);

			int getWidth();
			int getHeight();

		private:
			int width;
			int height;
			VkFormat format;

			VReference<VkImage> image;
			VReference<VkDeviceMemory> imageMemory;
			ImageView imageView;
		};
	}
}