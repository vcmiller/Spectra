#include "Light.h"
#include "Vulkan.h"
#include "LogicalDevice.h"

namespace spectra {
	Light::Light() {
		color = Color(1, 1, 1, 1);
		intensity = 1.0f;
		range = 10.0f;
		directional = true;

		allLights.add(this);

		createInfoBuffer();
		createDescriptorSet();
	}

	Light::~Light() {
		allLights.removeItem(this);
	}

	int Light::numLights() {
		return allLights.length();
	}

	Light * Light::lightAt(int index) {
		return allLights[index];
	}

	internal::VReference<VkDescriptorSetLayout> Light::descriptorLayout;
	internal::VReference<VkDescriptorPool> Light::descriptorPool;
	List<Light*> Light::allLights;

	void Light::createInfoBuffer() {
		VkDeviceSize bufferSize = sizeof(LightInfo);

		matrixBuffer.init(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	}

	void Light::createDescriptorSet() {
		auto device = internal::Vulkan::getLogicalDevice();

		VkDescriptorSetLayout layouts[] = { descriptorLayout };
		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = descriptorPool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = layouts;

		if (vkAllocateDescriptorSets(device->getDevice(), &allocInfo, &descriptorSet) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate descriptor set!");
		}

		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = matrixBuffer.getBuffer();
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(LightInfo);

		VkWriteDescriptorSet descriptorWrite = {};

		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSet;
		descriptorWrite.dstBinding = 3;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bufferInfo;

		vkUpdateDescriptorSets(device->getDevice(), 1, &descriptorWrite, 0, nullptr);
	}

	void Light::updateInfoBuffer() {
		auto device = internal::Vulkan::getLogicalDevice();

		LightInfo ubo = {};

		if (directional) {
			Vector3 pos = transform.getForward();
			ubo.position = glm::vec4(pos.x, pos.y, pos.z, 0.0f);
		} else {
			Vector3 pos = transform.getPosition();
			ubo.position = glm::vec4(pos.x, pos.y, pos.z, 1.0f);
		}
		
		ubo.color = glm::vec3(color.r, color.g, color.b);
		ubo.intensity = intensity;
		ubo.range = range;

		void* data;
		vkMapMemory(device->getDevice(), matrixBuffer.getMemory(), 0, sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device->getDevice(), matrixBuffer.getMemory());
	}

	void Light::init() {
		createDescriptorSetLayout();
		createDescriptorPool();
	}

	void Light::updateAll() {
		for (auto light : allLights) {
			light->updateInfoBuffer();
		}
	}

	void Light::cleanup() {
		descriptorLayout.cleanup();
		descriptorPool.cleanup();
	}

	void Light::createDescriptorSetLayout() {
		internal::LogicalDevice *device = internal::Vulkan::getLogicalDevice();

		descriptorLayout.cleanup();

		descriptorLayout = internal::VReference<VkDescriptorSetLayout>(device->getDevice(), vkDestroyDescriptorSetLayout);

		VkDescriptorSetLayoutBinding descriptorLayoutBinding = {};
		descriptorLayoutBinding.binding = 3;
		descriptorLayoutBinding.descriptorCount = 1;
		descriptorLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorLayoutBinding.pImmutableSamplers = nullptr;
		descriptorLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

		VkDescriptorSetLayoutCreateInfo descriptorLayoutInfo = {};
		descriptorLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		descriptorLayoutInfo.bindingCount = 1;
		descriptorLayoutInfo.pBindings = &descriptorLayoutBinding;

		if (vkCreateDescriptorSetLayout(device->getDevice(), &descriptorLayoutInfo, nullptr, descriptorLayout.replace()) != VK_SUCCESS) {
			throw std::runtime_error("failed to create descriptor set layout!");
		}
	}

	void Light::createDescriptorPool() {
		auto device = internal::Vulkan::getLogicalDevice();

		descriptorPool.cleanup();

		descriptorPool = internal::VReference<VkDescriptorPool>(device->getDevice(), vkDestroyDescriptorPool);

		VkDescriptorPoolSize descriptorPoolSize = {};
		descriptorPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorPoolSize.descriptorCount = 10;

		VkDescriptorPoolCreateInfo descriptorPoolInfo = {};
		descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptorPoolInfo.poolSizeCount = 1;
		descriptorPoolInfo.pPoolSizes = &descriptorPoolSize;
		descriptorPoolInfo.maxSets = 10;
		descriptorPoolInfo.pNext = nullptr;
		descriptorPoolInfo.flags = 0;

		if (vkCreateDescriptorPool(device->getDevice(), &descriptorPoolInfo, nullptr, descriptorPool.replace()) != VK_SUCCESS) {
			throw std::runtime_error("failed to create descriptor pool!");
		}
	}
}
