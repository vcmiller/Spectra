#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include "RenderPass.h"
#include "Pipeline.h"
#include "Window.h"
#include "Buffer.h"
#include "Texture.h"
#include "Transform.h"
#include "Object.h"

#include <map>

namespace spectra {
	class MeshRenderer;
	class Camera;

	class Shader;

	class Material : public Object {
	public:
		Material(Shader *shader, Texture *texture);
		void check(Camera *camera, int pass);
		Shader *getShader();

	private:
		friend class MeshRenderer;
		void createDescriptorSet();

		Shader *shader;
		Texture *texture;

		VkDescriptorSet descriptorSet;
	};
}