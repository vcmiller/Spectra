#include "ImageView.h"
#include "Vulkan.h"
#include "LogicalDevice.h"
#include "Log.h"

namespace spectra {
	namespace internal {
		ImageView::ImageView() {}

		ImageView::~ImageView() {
		}

		void ImageView::init(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags) {
			LogicalDevice *device = Vulkan::getLogicalDevice();

			imageView.cleanup();
			imageView = VReference<VkImageView>(device->getDevice(), vkDestroyImageView);

			VkImageViewCreateInfo viewInfo = {};
			viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			viewInfo.image = image;
			viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			viewInfo.format = format;
			viewInfo.subresourceRange.aspectMask = aspectFlags;
			viewInfo.subresourceRange.baseMipLevel = 0;
			viewInfo.subresourceRange.levelCount = 1;
			viewInfo.subresourceRange.baseArrayLayer = 0;
			viewInfo.subresourceRange.layerCount = 1;

			if (vkCreateImageView(device->getDevice(), &viewInfo, nullptr, imageView.replace()) != VK_SUCCESS) {
				throw std::runtime_error("failed to create texture image view!");
			}

			created = true;
		}

		VkImageView ImageView::getImageView() {
			return imageView;
		}

		bool ImageView::isCreated() {
			return created;
		}
	}
}