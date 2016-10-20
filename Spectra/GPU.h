#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


#include "VReference.h"
#include "List.h"

namespace spectra {
	namespace internal {

		class GPU {
		public:
			static List<GPU*> enumerate();

			void bind();

		private:
			GPU(VkPhysicalDevice device);
			GPU(const GPU&);
			GPU(GPU&&);
			GPU();
			void operator=(const GPU&);

			VkPhysicalDeviceProperties properties;
			VkPhysicalDeviceFeatures features;
			VkPhysicalDevice physicalDevice;
			VReference<VkDevice> logicalDevice { vkDestroyDevice };

			static List<GPU*> pointers;
			static GPU *gpus;
			static bool enumerated;
		};
	}
}