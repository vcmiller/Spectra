#include "Shader.h"
#include "Log.h"
#include "Vulkan.h"

#include <vector>
#include <array>

namespace spectra {
	Shader::Shader(std::string name) {
		createShaderModules(name);
		createDescriptorSetLayout();
		createDescriptorPool();
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

	void Shader::createDescriptorSetLayout() {
		internal::LogicalDevice *device = internal::Vulkan::getLogicalDevice();

		descriptorSetLayout = internal::VReference<VkDescriptorSetLayout>(device->getDevice(), vkDestroyDescriptorSetLayout);

		VkDescriptorSetLayoutBinding uboLayoutBinding = {};
		uboLayoutBinding.binding = 0;
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.descriptorCount = 1;

		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

		VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
		samplerLayoutBinding.binding = 1;
		samplerLayoutBinding.descriptorCount = 1;
		samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayoutBinding.pImmutableSamplers = nullptr;
		samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };
		VkDescriptorSetLayoutCreateInfo layoutInfo = {};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = bindings.size();
		layoutInfo.pBindings = bindings.data();

		if (vkCreateDescriptorSetLayout(device->getDevice(), &layoutInfo, nullptr, descriptorSetLayout.replace()) != VK_SUCCESS) {
			throw std::runtime_error("failed to create descriptor set layout!");
		}
	}

	void Shader::createDescriptorPool() {
		auto device = internal::Vulkan::getLogicalDevice();

		descriptorPool = internal::VReference<VkDescriptorPool>(device->getDevice(), vkDestroyDescriptorPool);

		std::array<VkDescriptorPoolSize, 2> poolSizes = {};
		poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSizes[0].descriptorCount = 1;
		poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSizes[1].descriptorCount = 1;

		VkDescriptorPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = poolSizes.size();
		poolInfo.pPoolSizes = poolSizes.data();
		poolInfo.maxSets = 1;

		if (vkCreateDescriptorPool(device->getDevice(), &poolInfo, nullptr, descriptorPool.replace()) != VK_SUCCESS) {
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