#include "Material.h"
#include "Vulkan.h"
#include "LogicalDevice.h"

#include <chrono>
#include <array>
#include <glm/gtc/matrix_transform.hpp>

namespace spectra {
	Material::Material(Shader *shader, Texture *texture) {
		this->shader = shader;
		this->texture = texture;

		createDescriptorSet();
	}

	internal::Pipeline * Material::getPipeline(internal::Window * window) {
		return &pipelines[window];
	}

	void Material::checkPipeline(internal::Window * window) {
		internal::Pipeline &pipeline = pipelines[window];

		if (!pipeline.isInitialized()) {
			Log::log("Creating pipeline");
			createPipeline(pipeline, window);
		}
	}

	void Material::createPipeline(internal::Pipeline &pipeline, internal::Window *window) {
		pipeline.init(window, shader, window->getRenderPass());
	}

	void Material::createDescriptorSet() {
		auto device = internal::Vulkan::getLogicalDevice();

		VkDescriptorSetLayout layouts[] = { shader->materialLayout };
		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = shader->materialPool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = layouts;

		if (vkAllocateDescriptorSets(device->getDevice(), &allocInfo, &descriptorSet) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate descriptor set!");
		}

		VkDescriptorImageInfo imageInfo = {};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = texture->image.getImageView()->getImageView();
		imageInfo.sampler = texture->sampler.getSampler();

		VkWriteDescriptorSet descriptorWrite = {};

		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSet;
		descriptorWrite.dstBinding = 1;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pImageInfo = &imageInfo;

		vkUpdateDescriptorSets(device->getDevice(), 1, &descriptorWrite, 0, nullptr);
	}
}