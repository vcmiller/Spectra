#pragma once
#include "PhysicalDevice.h"
#include "CommandPool.h"

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
			const PhysicalDevice *getPhysicalDevice();
			CommandPool *getCommandPool();

			VkQueue getGraphicsQueue();
			VkQueue getPresentQueue();

			void waitIdle();

		private:
			void createDevice(Window* window);
			void createCommandPool(Window* window);

			VReference<VkDevice> device { vkDestroyDevice };
			CommandPool commandPool;

			const PhysicalDevice *physicalDevice;

			VkQueue graphicsQueue;
			VkQueue presentQueue;
		};
	}
}