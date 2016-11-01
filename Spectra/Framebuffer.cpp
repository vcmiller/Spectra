#include "Framebuffer.h"
#include "Vulkan.h"
#include "LogicalDevice.h"
#include "RenderPass.h"

#include <array>

namespace spectra {
	namespace internal {
		Framebuffer::Framebuffer() {}

		void Framebuffer::init(int width, int height, RenderPass *renderPass, ImageView *color, ImageView * depth) {
			this->width = width;
			this->height = height;

			LogicalDevice *device = Vulkan::getLogicalDevice();

			buffer.cleanup();
			buffer = VReference<VkFramebuffer>(device->getDevice(), vkDestroyFramebuffer);

			VkFramebufferCreateInfo framebufferInfo = {};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = renderPass->getRenderPass();
			framebufferInfo.width = width;
			framebufferInfo.height = height;
			framebufferInfo.layers = 1;

			std::array<VkImageView, 2> attachments;

			if (depth) {
				attachments[0] = color->getImageView();
				attachments[1] = depth->getImageView();

				framebufferInfo.attachmentCount = attachments.size();
				framebufferInfo.pAttachments = attachments.data();
			} else {
				attachments[0] = color->getImageView();

				framebufferInfo.attachmentCount = 1;
				framebufferInfo.pAttachments = attachments.data();
			}

			if (vkCreateFramebuffer(device->getDevice(), &framebufferInfo, nullptr, buffer.replace()) != VK_SUCCESS) {
				throw std::runtime_error("failed to create framebuffer!");
			}
		}

		VkFramebuffer Framebuffer::getBuffer() {
			return buffer;
		}

		int Framebuffer::getWidth() {
			return width;
		}

		int Framebuffer::getHeight() {
			return height;
		}
	}
}