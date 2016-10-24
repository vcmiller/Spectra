#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include "RenderPass.h"
#include "Pipeline.h"
#include "Shader.h"
#include "Window.h"
#include "Buffer.h"
#include "Texture.h"
#include "Transform.h"

namespace spectra {
	class MeshRenderer;

	class Material {
	public:
		Material(internal::Window *window, Shader *shader, Texture *texture);

	private:
		friend class MeshRenderer;
		void createPipeline();
		void createUniformBuffer();
		void createDescriptorSet();

		void updateUniformBuffer(Transform *transform);

		Shader *shader;
		Texture *texture;
		internal::Window *window;

		internal::Pipeline pipeline;
		internal::Buffer uniformBuffer;

		VkDescriptorSet descriptorSet;

		struct UniformBufferObject {
			glm::mat4 model;
			glm::mat4 view;
			glm::mat4 proj;
		};
	};
}