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
#include "Map.h"

namespace spectra {
	class MeshRenderer;

	class Material {
	public:
		Material(internal::Window *window, Shader *shader, Texture *texture);

	private:
		friend class MeshRenderer;
		void createPipeline();
		void createDescriptorSet();

		Shader *shader;
		Texture *texture;
		internal::Window *window;

		internal::Pipeline pipeline;
		Map<internal::Window *, internal::Pipeline> pipelines;


		VkDescriptorSet descriptorSet;
	};
}