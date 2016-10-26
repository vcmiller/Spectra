#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VReference.h"
#include "LogicalDevice.h"
#include "ImageView.h"
#include "Image.h"
#include "RenderPass.h"
#include "Framebuffer.h"
#include "Camera.h"

#include <string>
#include <vector>

namespace spectra {
	class Spectra;

	namespace internal {
		class Window {
		public:
			Window(int width, int height, std::string name, bool resizeable = false, bool complete = true, bool repaintOnRender = true);
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
			void repaint();

			int getWidth();

			int getHeight();

			uint32_t getCurrentImage();

			VkSemaphore getImageSemaphore();

			CommandPool *getCommandPool();

		private:
			friend class Spectra;
			friend class Camera;

			void createSurface();
			void createSwapChain();
			void createImageViews();
			void createDepthImage();
			void createRenderPass();
			void createFramebuffers();
			void createSemafores();

			void recreateSwapChain();

			void acquireNextImage();
			void display();

			VReference<VkSurfaceKHR> surface;
			VReference<VkSwapchainKHR> swapChain;
			std::vector<VkImage> swapChainImages;
			std::vector<ImageView> swapChainImageViews;
			Image depthImage;
			RenderPass renderPass;
			std::vector<Framebuffer> framebuffers;

			VReference<VkSemaphore> imageAvailableSemaphore;

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

			bool repaintOnRender;

			List<Camera *> cameras;

			static void resized(GLFWwindow* window, int width, int height);

			static Window *main;
			static List<Window*> allWindows;
		};
	}
}