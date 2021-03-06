#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Vulkan.h"
#include "LogicalDevice.h"

namespace spectra {
	Texture::Texture(std::string file) {
		int texWidth, texHeight, texChannels;
		stbi_uc* pixels = stbi_load(file.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		VkDeviceSize imageSize = texWidth * texHeight * 4;

		if (!pixels) {
			throw std::runtime_error("failed to load texture image!");
		}

		internal::Image stagingImage;
		stagingImage.init(texWidth, texHeight, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_LINEAR, VK_IMAGE_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, VK_SAMPLE_COUNT_1_BIT);

		auto device = internal::Vulkan::getLogicalDevice();

		void* data;
		vkMapMemory(device->getDevice(), stagingImage.getMemory(), 0, imageSize, 0, &data);
		memcpy(data, pixels, (size_t)imageSize);
		vkUnmapMemory(device->getDevice(), stagingImage.getMemory());

		stbi_image_free(pixels);

		image.init(texWidth, texHeight, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_SAMPLE_COUNT_1_BIT);

		stagingImage.transition(VK_IMAGE_LAYOUT_PREINITIALIZED, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
		image.transition(VK_IMAGE_LAYOUT_PREINITIALIZED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		internal::Image::copy(&stagingImage, &image);

		image.transition(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		image.createImageView(VK_IMAGE_ASPECT_COLOR_BIT);

		sampler.init();
	}
}