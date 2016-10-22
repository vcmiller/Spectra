#pragma once

#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VReference.h"
#include "LogicalDevice.h"
#include "ImageView.h"

#include <string>
#include <vector>

namespace spectra {
	class Spectra;

	namespace internal {
		class Window {
		public:
			Window(int width, int height, std::string name, bool resizeable = false);
			~Window();

			VkSurfaceKHR getSurface();
			LogicalDevice *getDevice();

			static Window *getMainWindow();

			bool closeRequested();
			static void pollEvents();

		private:
			friend class Spectra;

			void createSurface();
			void createLogicalDevice();
			void createSwapChain();
			void createImageViews();
			void recreateSwapChain();

			VReference<VkSurfaceKHR> surface;
			LogicalDevice device;
			VReference<VkSwapchainKHR> swapChain;
			std::vector<VkImage> swapChainImages;
			std::vector<ImageView> swapChainImageViews;

			VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
			VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
			VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR & capabilities);

			VkFormat swapChainImageFormat;
			VkExtent2D swapChainExtent;

			GLFWwindow *window;

			int width;
			int height;

			static void resized(GLFWwindow* window, int width, int height);

			static Window *main;
		};
	}
}