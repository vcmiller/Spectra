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

#include <map>

namespace spectra {
	class MeshRenderer;

	class Material {
	public:
		Material(Shader *shader, Texture *texture);

		internal::Pipeline *getPipeline(internal::Window *window);
		void checkPipeline(internal::Window *window);

	private:
		friend class MeshRenderer;
		void createPipeline(internal::Pipeline &pipeline, internal::Window *window);
		void createDescriptorSet();

		Shader *shader;
		Texture *texture;

		std::map<internal::Window *, internal::Pipeline> pipelines;


		VkDescriptorSet descriptorSet;
	};
}