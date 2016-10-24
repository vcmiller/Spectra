#pragma once

#include "Vertex.h"
#include "Buffer.h"
#include "List.h"

#include <string>

namespace spectra {
	class Mesh {
	public:
		Mesh();
		Mesh(std::string file);

		List<Vertex> vertices;
		List<int> indices;

		void createBuffers();

		internal::Buffer* getVertexBuffer();
		internal::Buffer* getIndexBuffer();
	private:

		void createVertexBuffer();
		void createIndexBuffer();

		internal::Buffer vertexBuffer;
		internal::Buffer indexBuffer;
	};
}