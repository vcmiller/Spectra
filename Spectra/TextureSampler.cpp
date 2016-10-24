#include "TextureSampler.h"
#include "Vulkan.h"
#include "LogicalDevice.h"

namespace spectra {
	namespace internal {
		TextureSampler::TextureSampler() {}

		void TextureSampler::init(bool repeat, bool linear, int maxAntistropy) {
			textureSampler = VReference<VkSampler>(Vulkan::getLogicalDevice()->getDevice(), vkDestroySampler);

			VkSamplerCreateInfo samplerInfo = {};
			samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
			samplerInfo.magFilter = VK_FILTER_LINEAR;
			samplerInfo.minFilter = VK_FILTER_LINEAR;

			samplerInfo.addressModeU = repeat ? VK_SAMPLER_ADDRESS_MODE_REPEAT : VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			samplerInfo.addressModeV = repeat ? VK_SAMPLER_ADDRESS_MODE_REPEAT : VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			samplerInfo.addressModeW = repeat ? VK_SAMPLER_ADDRESS_MODE_REPEAT : VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;

			samplerInfo.anisotropyEnable = VK_TRUE;
			samplerInfo.maxAnisotropy = maxAntistropy;

			samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;

			samplerInfo.unnormalizedCoordinates = VK_FALSE;

			samplerInfo.compareEnable = VK_FALSE;
			samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

			samplerInfo.mipmapMode = linear ? VK_SAMPLER_MIPMAP_MODE_LINEAR : VK_SAMPLER_MIPMAP_MODE_NEAREST;
			samplerInfo.mipLodBias = 0.0f;
			samplerInfo.minLod = 0.0f;
			samplerInfo.maxLod = 0.0f;

			if (vkCreateSampler(Vulkan::getLogicalDevice()->getDevice(), &samplerInfo, nullptr, textureSampler.replace()) != VK_SUCCESS) {
				throw std::runtime_error("failed to create texture sampler!");
			}
		}

		VkSampler TextureSampler::getSampler() {
			return textureSampler;
		}
	}
}