#include "GPU.h"
#include "Log.h"
#include "Vulkan.h"

#include <vector>

namespace spectra {
	namespace internal {


		spectra::List<GPU*> GPU::enumerate() {
			if (enumerated) {
				return pointers;
			} else {
				uint32_t deviceCount = 0;
				vkEnumeratePhysicalDevices(Vulkan::getInstance(), &deviceCount, nullptr);

				if (deviceCount == 0) {
					throw std::runtime_error("No supported GPU detected");
				}

				std::vector<VkPhysicalDevice> devices(deviceCount);
				vkEnumeratePhysicalDevices(Vulkan::getInstance(), &deviceCount, devices.data());

				gpus = new GPU[deviceCount];
				
				for (int i = 0; i < deviceCount; i++) {
					gpus[i] = GPU()
				}
			}

		}

		void GPU::bind() {

		}

		GPU::GPU(VkPhysicalDevice device) {
			physicalDevice = device;

			vkGetPhysicalDeviceProperties(device, &properties);
			vkGetPhysicalDeviceFeatures(device, &features);
		}

		GPU::GPU(const GPU&) { }
		GPU::GPU(GPU&&) { }
		GPU::GPU() { }
		void GPU::operator=(const GPU&) {}

		List<GPU*> GPU::pointers;
		GPU *GPU::gpus;
		bool GPU::enumerated = false;

	}
}