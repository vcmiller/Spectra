#include "Shader.h"
#include "Log.h"
#include "Vulkan.h"

#include <vector>
#include <array>

namespace spectra {
	Shader::Shader(std::string name) {
		createShaderModules(name);
		createDescriptorSetLayouts();
		createDescriptorPools();
	}

	void Shader::createShaderModules(std::string name) {
		auto vertShaderCode = readFile(name + "-vert.spv");
		auto fragShaderCode = readFile(name + "-frag.spv");

		internal::LogicalDevice *device = internal::Vulkan::getLogicalDevice();

		vertexModule.cleanup();
		fragmentModule.cleanup();

		vertexModule = internal::VReference<VkShaderModule>(device->getDevice(), vkDestroyShaderModule);
		fragmentModule = internal::VReference<VkShaderModule>(device->getDevice(), vkDestroyShaderModule);

		vertexModule = createShaderModule(vertShaderCode);
		fragmentModule = createShaderModule(fragShaderCode);
	}

	void Shader::createDescriptorSetLayouts() {
		internal::LogicalDevice *device = internal::Vulkan::getLogicalDevice();

		matricesLayout.cleanup();
		materialLayout.cleanup();

		materialLayout = internal::VReference<VkDescriptorSetLayout>(device->getDevice(), vkDestroyDescriptorSetLayout);
		matricesLayout = internal::VReference<VkDescriptorSetLayout>(device->getDevice(), vkDestroyDescriptorSetLayout);

		VkDescriptorSetLayoutBinding matricesLayoutBinding = {};
		matricesLayoutBinding.binding = 0;
		matricesLayoutBinding.descriptorCount = 1;
		matricesLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		matricesLayoutBinding.pImmutableSamplers = nullptr;
		matricesLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

		VkDescriptorSetLayoutBinding materialLayoutBinding = {};
		materialLayoutBinding.binding = 1;
		materialLayoutBinding.descriptorCount = 1;
		materialLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		materialLayoutBinding.pImmutableSamplers = nullptr;
		materialLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		VkDescriptorSetLayoutCreateInfo matricesLayoutInfo = {};
		matricesLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		matricesLayoutInfo.bindingCount = 1;
		matricesLayoutInfo.pBindings = &matricesLayoutBinding;

		VkDescriptorSetLayoutCreateInfo materialLayoutInfo = {};
		materialLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		materialLayoutInfo.bindingCount = 1;
		materialLayoutInfo.pBindings = &materialLayoutBinding;

		if (vkCreateDescriptorSetLayout(device->getDevice(), &matricesLayoutInfo, nullptr, matricesLayout.replace()) != VK_SUCCESS ||
			vkCreateDescriptorSetLayout(device->getDevice(), &materialLayoutInfo, nullptr, materialLayout.replace()) != VK_SUCCESS) {
			throw std::runtime_error("failed to create descriptor set layout!");
		}
	}

	void Shader::createDescriptorPools() {
		auto device = internal::Vulkan::getLogicalDevice();

		matricesPool.cleanup();
		materialPool.cleanup();

		matricesPool = internal::VReference<VkDescriptorPool>(device->getDevice(), vkDestroyDescriptorPool);
		materialPool = internal::VReference<VkDescriptorPool>(device->getDevice(), vkDestroyDescriptorPool);

		VkDescriptorPoolSize matricesPoolSize = {};
		matricesPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		matricesPoolSize.descriptorCount = 5;

		VkDescriptorPoolSize materialPoolSize = {};
		materialPoolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		materialPoolSize.descriptorCount = 5;

		VkDescriptorPoolCreateInfo matricesPoolInfo = {};
		matricesPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		matricesPoolInfo.poolSizeCount = 1;
		matricesPoolInfo.pPoolSizes = &matricesPoolSize;
		matricesPoolInfo.maxSets = 5;
		matricesPoolInfo.pNext = nullptr;
		matricesPoolInfo.flags = 0;

		VkDescriptorPoolCreateInfo materialPoolInfo = {};
		materialPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		materialPoolInfo.poolSizeCount = 1;
		materialPoolInfo.pPoolSizes = &materialPoolSize;
		materialPoolInfo.maxSets = 5;
		materialPoolInfo.pNext = nullptr;
		materialPoolInfo.flags = 0;

		if (vkCreateDescriptorPool(device->getDevice(), &matricesPoolInfo, nullptr, matricesPool.replace()) != VK_SUCCESS ||
			vkCreateDescriptorPool(device->getDevice(), &materialPoolInfo, nullptr, materialPool.replace()) != VK_SUCCESS) {
			throw std::runtime_error("failed to create descriptor pool!");
		}
	}

	VkShaderModule Shader::createShaderModule(const std::vector<char>& code) {
		internal::LogicalDevice *device = internal::Vulkan::getLogicalDevice();

		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = (uint32_t*)code.data();

		VkShaderModule module;

		if (vkCreateShaderModule(device->getDevice(), &createInfo, nullptr, &module) != VK_SUCCESS) {
			throw std::runtime_error("failed to create shader module!");
		}

		return module;
	}

	std::vector<char> Shader::readFile(const std::string& filename) {
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		if (!file.is_open()) {
			throw std::runtime_error("failed to open file!");
		}

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return buffer;
	}
}