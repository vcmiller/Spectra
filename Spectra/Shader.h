#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VReference.h"
#include "LogicalDevice.h"

#include <string>
#include <map>

#include "Pipeline.h"

namespace spectra {

	class Material;
	class MeshRenderer;
	class Camera;

	class Shader {
	public:
		Shader(std::string name);

		internal::Pipeline *getPipeline(Camera *camera, int pass);
		void checkPipeline(Camera *camera, int pass);

	private:
		friend class internal::Pipeline;
		friend class Material;
		friend class MeshRenderer;

		void createShaderModules(std::string name);
		void createDescriptorSetLayouts();
		void createDescriptorPools();
		void createPipeline(internal::Pipeline *pipeline, Camera *camera, int pass);

		static VkShaderModule createShaderModule(const std::vector<char>& code);
		static std::vector<char> readFile(const std::string & filename);

		internal::VReference<VkShaderModule> vertexModule;
		std::vector<internal::VReference<VkShaderModule>> fragmentModules;
		internal::VReference<VkDescriptorSetLayout> materialLayout;
		internal::VReference<VkDescriptorSetLayout> matricesLayout;
		internal::VReference<VkDescriptorPool> materialPool;
		internal::VReference<VkDescriptorPool> matricesPool;

		
		std::vector<std::map<Camera *, internal::Pipeline>> passes;
	};
}