#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/gtx/hash.hpp>

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

		bool operator==(const Vertex &other) const;

	private:
		friend class internal::Pipeline;

		static VkVertexInputBindingDescription getBindingDescription();

		static std::array<VkVertexInputAttributeDescription, 4> getAttributeDescriptions();
	};
}

namespace std {
	template<> struct hash<spectra::Vertex> {
		size_t operator()(spectra::Vertex const& vertex) const {
			spectra::Vector3 position = vertex.position;
			spectra::Vector3 normal = vertex.normal;
			spectra::Vector2 texcoord = vertex.texcoord;

			return ((hash<glm::vec3>()(glm::vec3(position.x, position.y, position.z)) ^
				(hash<glm::vec3>()(glm::vec3(normal.x, normal.y, normal.z)) << 1)) >> 1) ^
				(hash<glm::vec2>()(glm::vec2(texcoord.x, texcoord.y)) << 1);
		}
	};
}