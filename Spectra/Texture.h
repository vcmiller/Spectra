#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VReference.h"
#include "Image.h"
#include "TextureSampler.h"

#include <string>

namespace spectra {
	class Material;

	class Texture {
	public:
		Texture(std::string file);

	private:
		friend class Material;

		internal::Image image;
		internal::TextureSampler sampler;
	};
}