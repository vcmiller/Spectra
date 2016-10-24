#include "Mesh.h"
#include "Vulkan.h"
#include "LogicalDevice.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <unordered_map>

namespace spectra {

	Mesh::Mesh() {}

	Mesh::Mesh(std::string file) {
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string err;

		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, file.c_str())) {
			throw std::runtime_error(err);
		}

		std::unordered_map<Vertex, int> uniqueVertices = {};

		for (const auto& shape : shapes) {
			for (const auto& index : shape.mesh.indices) {
				Vertex vertex;

				vertex.position = Vector3(
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2]
				);

				if (attrib.normals.size() > 0) {
					vertex.normal = Vector3(
						attrib.normals[3 * index.normal_index + 0],
						attrib.normals[3 * index.normal_index + 1],
						attrib.normals[3 * index.normal_index + 2]
					);
				}

				if (attrib.texcoords.size() > 0) {
					vertex.texcoord = Vector2(
						attrib.texcoords[2 * index.texcoord_index + 0],
						1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
					);
				}

				if (uniqueVertices.count(vertex) == 0) {
					uniqueVertices[vertex] = vertices.length();
					vertices.add(vertex);
				}

				indices.add(uniqueVertices[vertex]);
			}
		}

		createBuffers();
	}

	void Mesh::createBuffers() {
		createVertexBuffer();
		createIndexBuffer();
	}

	internal::Buffer *Mesh::getVertexBuffer() {
		return &vertexBuffer;
	}

	internal::Buffer *Mesh::getIndexBuffer() {
		return &indexBuffer;
	}

	void Mesh::createVertexBuffer() {
		vertexBuffer.init(vertices, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
	}

	void Mesh::createIndexBuffer() {
		indexBuffer.init(indices, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
	}
}
