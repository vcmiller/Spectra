#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VReference.h"
#include "LogicalDevice.h"
#include "ImageView.h"
#include "Image.h"
#include "RenderPass.h"
#include "Framebuffer.h"

#include <string>
#include <vector>

namespace spectra {
	class Spectra;

	namespace internal {
		class Window {
		public:
			Window(int width, int height, std::string name, bool resizeable = false, bool complete = true);
			~Window();

			static void init();

			VkSurfaceKHR getSurface();
			VkSwapchainKHR getSwapChain();

			VkFormat getSwapChainImageFormat();
			VkExtent2D getSwapChainExtent();
			VkFormat getDepthFormat();
			RenderPass *getRenderPass();

			int getNumFramebuffers();
			Framebuffer *getFramebuffer(int i);

			static Window *getMainWindow();

			bool closeRequested();
			static void pollEvents();

			void complete();

			void display();

			int getWidth();

			int getHeight();

			void acquireNextImage();
			uint32_t getCurrentImage();

			VkSemaphore getImageSemaphore();
			VkSemaphore getRenderSemaphore();
			VkFence getRenderFence();

			CommandPool *getCommandPool();

		private:
			friend class Spectra;

			void createSurface();
			void createSwapChain();
			void createImageViews();
			void createDepthImage();
			void createRenderPass();
			void createFramebuffers();
			void createSemafores();

			void recreateSwapChain();

			VReference<VkSurfaceKHR> surface;
			VReference<VkSwapchainKHR> swapChain;
			std::vector<VkImage> swapChainImages;
			std::vector<ImageView> swapChainImageViews;
			Image depthImage;
			RenderPass renderPass;
			std::vector<Framebuffer> framebuffers;

			VReference<VkSemaphore> imageAvailableSemaphore;
			VReference<VkSemaphore> renderFinishedSemaphore;
			VReference<VkFence> renderFinishedFence;

			std::vector<CommandPool> commandPools;
			CommandPool *currentCommandPool;

			VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
			VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
			VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR & capabilities);

			VkFormat swapChainImageFormat;
			VkExtent2D swapChainExtent;

			uint32_t currentImage;

			GLFWwindow *window;

			int width;
			int height;

			static void resized(GLFWwindow* window, int width, int height);

			static Window *main;
		};
	}
}