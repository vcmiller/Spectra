#pragma once
#include "PhysicalDevice.h"

#include <vector>
#include <set>

namespace spectra {
	namespace internal {
		class Window;

		class LogicalDevice {
		public:
			LogicalDevice();
			~LogicalDevice();
			void init(const PhysicalDevice* physicalDevice, Window* window);
			VkDevice getDevice();

		private:
			VReference<VkDevice> device { vkDestroyDevice };
			const PhysicalDevice *physicalDevice;

			VkQueue graphicsQueue;
			VkQueue presentQueue;
		};
	}
}