#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <array>

#include "Vector.h"

namespace spectra {
	namespace internal {
		class Pipeline;
	}

	class Vertex {
	public:
		internal::Vector<3, Vector3, glm::vec3> position;
		internal::Vector<2, Vector2, glm::vec2> texcoord;
		internal::Vector<3, Vector3, glm::vec3> normal;
		internal::Vector<3, Vector3, glm::vec3> tangent;

	private:
		friend class internal::Pipeline;

		static VkVertexInputBindingDescription getBindingDescription();

		static std::array<VkVertexInputAttributeDescription, 4> getAttributeDescriptions();
	};
}