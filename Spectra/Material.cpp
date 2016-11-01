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

	internal::Pipeline * Material::getPipeline(Camera * camera) {
		return &pipelines[camera];
	}

	void Material::checkPipeline(Camera * camera) {

		internal::Pipeline &pipeline = pipelines[camera];

		if (!pipeline.isInitialized() || pipeline.outOfDate()) {
			Log::log("Creating pipeline");
			createPipeline(pipeline, camera);
		}
	}

	void Material::createPipeline(internal::Pipeline &pipeline, Camera * camera) {
		pipeline.init(camera, shader, camera->getRenderPass());
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
		descriptorWrite.dstBinding = 2;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pImageInfo = &imageInfo;

		vkUpdateDescriptorSets(device->getDevice(), 1, &descriptorWrite, 0, nullptr);
	}
}