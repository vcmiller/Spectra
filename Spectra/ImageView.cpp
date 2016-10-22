#include "ImageView.h"
#include "Vulkan.h"
#include "LogicalDevice.h"
#include "Log.h"

namespace spectra {
	namespace internal {
		ImageView::ImageView() {}

		ImageView::~ImageView() {
			vkDestroyImageView(device, imageView, nullptr);
		}

		ImageView::ImageView(VkImage image, LogicalDevice *device, VkFormat format, VkImageAspectFlags aspectFlags) {
			init(image, device, format, aspectFlags);
		}

		void ImageView::init(VkImage image, LogicalDevice *device, VkFormat format, VkImageAspectFlags aspectFlags) {
			this->device = device->getDevice();;

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

			if (vkCreateImageView(device->getDevice(), &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
				throw std::runtime_error("failed to create texture image view!");
			}
		}

		VkImageView ImageView::getImageView() {
			return VkImageView();
		}
	}
}