#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VReference.h"
#include "LogicalDevice.h"
#include "Vulkan.h"
#include "Mesh.h"
#include "Material.h"
#include "Component.h"
#include "Buffer.h"
#include "Window.h"
#include "List.h"
#include "CommandBuffer.h"

#include <vector>

namespace spectra {
	class MeshRenderer : public Component {
	public:
		MeshRenderer();
		virtual ~MeshRenderer();

		void init(Mesh *mesh, Material *material);

		void render(int pass);
		void preRender(int pass);

	private:

		Mesh *mesh;
		Material *material;

		void createMatrixBuffer();
		void createDescriptorSet();
		void updateMatrixBuffer();

		internal::Buffer matrixBuffer;
		VkDescriptorSet descriptorSet;


		struct ObjectMatrices {
			glm::mat4 model;
		};
	};
}