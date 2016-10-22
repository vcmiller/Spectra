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

	class Shader {
	public:
		Shader(std::string name, internal::LogicalDevice *device);

	private:
		friend class internal::Pipeline;

		void createShaderModules(std::string name, internal::LogicalDevice *device);
		void createDescriptorSetLayout(internal::LogicalDevice *device);

		static VkShaderModule createShaderModule(const std::vector<char>& code, internal::LogicalDevice *device);
		static std::vector<char> readFile(const std::string & filename);

		internal::VReference<VkShaderModule> vertexModule;
		internal::VReference<VkShaderModule> fragmentModule;
		internal::VReference<VkDescriptorSetLayout> descriptorSetLayout;
	};
}