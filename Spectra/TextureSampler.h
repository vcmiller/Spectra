#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VReference.h"

namespace spectra {
	namespace internal {
		class TextureSampler {
		public:
			TextureSampler();

			void init(bool repeat = true, bool linear = true, int maxAntistropy = 16);

			VkSampler getSampler();

		private:

			VReference<VkSampler> textureSampler;
		};

	}
}