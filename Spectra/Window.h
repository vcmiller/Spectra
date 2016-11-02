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
#include "WindowInputHandler.h"
#include "CursorMode.h"

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

			int getNumFramebuffers();
			Framebuffer *getFramebuffer(int i);
			CommandBuffer *getCommandBuffer();

			static Window *getMainWindow();

			bool closeRequested();
			static void pollEvents();

			void complete(int width, int height);
			void repaint();

			int getWidth();

			int getHeight();

			void setCursorMode(CursorMode mode);

			uint32_t getCurrentImage();

			VkSemaphore getImageSemaphore();

			CommandPool *getCommandPool();

			const WindowInputHandler &input{ inputHandler };

		private:
			friend class Spectra;
			friend class Camera;

			void createSurface();
			void createSwapChain(bool first, int width, int height);
			void createImageViews();
			void createDepthImage();
			void createRenderPass();
			void createFramebuffers();
			void createSemafores();

			void onResized(int width, int height);

			void acquireNextImage();
			void display();

			void submitBuffer(internal::CommandBuffer *cmd);

			VReference<VkSurfaceKHR> surface;
			VReference<VkSwapchainKHR> swapChain;
			std::vector<VkImage> swapChainImages;
			std::vector<ImageView> swapChainImageViews;
			Image depthImage;
			RenderPass renderPass;
			std::vector<Framebuffer> framebuffers;

			VReference<VkSemaphore> imageAvailableSemaphore;

			std::vector<VReference<VkSemaphore>> renderFinishedSemaphores;
			std::vector<VReference<VkFence>> renderFinishedFences;

			std::vector<CommandPool> commandPools;
			CommandPool *currentCommandPool;

			std::vector<CommandBuffer> commandBuffers;

			VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
			VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
			VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR & capabilities, int width, int height);

			VkFormat swapChainImageFormat;
			VkExtent2D swapChainExtent;

			uint32_t currentImage;

			GLFWwindow *window;

			WindowInputHandler inputHandler;

			bool repaintOnRender;

			List<Camera *> cameras;

			static void resized(GLFWwindow* window, int width, int height);
			static void keyEvent(GLFWwindow *window, int key, int scancode, int action, int mods);
			static void charEvent(GLFWwindow *window, unsigned int codepoint);
			static void mouseMoveEvent(GLFWwindow *window, double x, double y);
			static void mouseButtonEvent(GLFWwindow *window, int button, int action, int mods);
			static void mouseWheelEvent(GLFWwindow *window, double x, double y);

			static void closeAll();

			static Window *main;
			static List<Window*> allWindows;
		};
	}
}