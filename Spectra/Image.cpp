#include "Image.h"
#include "LogicalDevice.h"
#include "Vulkan.h"
#include "CommandBuffer.h"

namespace spectra {
	namespace internal {
		Image::Image() {}

		void Image::init(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkSampleCountFlagBits sampleCount) {
			this->format = format;
			this->width = width;
			this->height = height;
			
			image.cleanup();
			imageMemory.cleanup();

			LogicalDevice *device = Vulkan::getLogicalDevice();

			image = VReference<VkImage>(device->getDevice(), vkDestroyImage);
			imageMemory = VReference<VkDeviceMemory>(device->getDevice(), vkFreeMemory);
			
			VkImageCreateInfo imageInfo = {};
			imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			imageInfo.imageType = VK_IMAGE_TYPE_2D;
			imageInfo.extent.width = width;
			imageInfo.extent.height = height;
			imageInfo.extent.depth = 1;
			imageInfo.mipLevels = 1;
			imageInfo.arrayLayers = 1;
			imageInfo.format = format;
			imageInfo.tiling = tiling;
			imageInfo.initialLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;
			imageInfo.usage = usage;
			imageInfo.samples = sampleCount;
			imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

			if (vkCreateImage(device->getDevice(), &imageInfo, nullptr, image.replace()) != VK_SUCCESS) {
				throw std::runtime_error("failed to create image!");
			}

			VkMemoryRequirements memRequirements;
			vkGetImageMemoryRequirements(device->getDevice(), image, &memRequirements);

			VkMemoryAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocInfo.allocationSize = memRequirements.size;
			allocInfo.memoryTypeIndex = Vulkan::findMemoryType(memRequirements.memoryTypeBits, properties);

			if (vkAllocateMemory(device->getDevice(), &allocInfo, nullptr, imageMemory.replace()) != VK_SUCCESS) {
				throw std::runtime_error("failed to allocate image memory!");
			}

			vkBindImageMemory(device->getDevice(), image, imageMemory, 0);
		}


		ImageView *Image::getImageView() {
			return &imageView;
		}

		void Image::createImageView(VkImageAspectFlags aspectFlags) {
			imageView.init(image, format, aspectFlags);
		}

		void Image::transition(VkImageLayout oldLayout, VkImageLayout newLayout) {
			CommandBuffer commandBuffer(true);
			commandBuffer.begin();

			VkImageMemoryBarrier barrier = {};
			barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			barrier.oldLayout = oldLayout;
			barrier.newLayout = newLayout;

			barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

			barrier.image = image;

			if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
				barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

				if (hasStencil(format)) {
					barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
				}
			} else {
				barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			}

			barrier.subresourceRange.baseMipLevel = 0;
			barrier.subresourceRange.levelCount = 1;
			barrier.subresourceRange.baseArrayLayer = 0;
			barrier.subresourceRange.layerCount = 1;

			if (oldLayout == VK_IMAGE_LAYOUT_PREINITIALIZED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL) {
				barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
				barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			} else if (oldLayout == VK_IMAGE_LAYOUT_PREINITIALIZED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
				barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
				barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			} else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
				barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
			} else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
				barrier.srcAccessMask = 0;
				barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			} else {
				throw std::invalid_argument("unsupported layout transition!");
			}

			vkCmdPipelineBarrier(commandBuffer.getBuffer(), VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

			commandBuffer.end();
			commandBuffer.submit();
		}

		bool Image::hasStencil(VkFormat format) {
			return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
		}

		VkImage Image::getImage() {
			return image;
		}

		VkDeviceMemory Image::getMemory() {
			return imageMemory;
		}

		void Image::copy(Image *src, Image *dest) {
			CommandBuffer cmd(true);
			cmd.begin();

			VkImageSubresourceLayers subResource = {};
			subResource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			subResource.baseArrayLayer = 0;
			subResource.mipLevel = 0;
			subResource.layerCount = 1;

			VkImageCopy region = {};
			region.srcSubresource = subResource;
			region.dstSubresource = subResource;
			region.srcOffset = { 0, 0, 0 };
			region.dstOffset = { 0, 0, 0 };
			region.extent.width = src->width;
			region.extent.height = src->height;
			region.extent.depth = 1;

			vkCmdCopyImage(cmd.getBuffer(), src->getImage(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dest->getImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

			cmd.end();
			cmd.submit();
		}

		int Image::getWidth() {
			return width;
		}

		int Image::getHeight() {
			return height;
		}
	}
}