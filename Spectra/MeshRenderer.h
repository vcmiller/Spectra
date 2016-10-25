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

		void init(Mesh *mesh, Material *material, internal::Window *window);

		void render();

	private:

		Mesh *mesh;
		Material *material;
		internal::Window *window;

		void createCommandBuffer();

		internal::CommandBuffer drawCmd;
	};
}