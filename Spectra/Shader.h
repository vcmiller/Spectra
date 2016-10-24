#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VReference.h"
#include "LogicalDevice.h"

#include <string>

namespace spectra {
	namespace internal {
		class Pipeline;
	}

	class Material;

	class Shader {
	public:
		Shader(std::string name);

	private:
		friend class internal::Pipeline;
		friend class Material;

		void createShaderModules(std::string name);
		void createDescriptorSetLayout();
		void createDescriptorPool();

		static VkShaderModule createShaderModule(const std::vector<char>& code);
		static std::vector<char> readFile(const std::string & filename);

		internal::VReference<VkShaderModule> vertexModule;
		internal::VReference<VkShaderModule> fragmentModule;
		internal::VReference<VkDescriptorSetLayout> descriptorSetLayout;
		internal::VReference<VkDescriptorPool> descriptorPool;
	};
}