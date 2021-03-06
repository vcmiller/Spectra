#include "Window.h"
#include "FMath.h"
#include "Vulkan.h"
#include "PhysicalDevice.h"

namespace spectra {
	namespace internal {
		Window::Window(int width, int height, std::string name, bool resizeable, bool fullscreen, bool complete, bool repaintOnRender) : surface(Vulkan::getInstance(), vkDestroySurfaceKHR) {
			this->repaintOnRender = repaintOnRender;

			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			glfwWindowHint(GLFW_RESIZABLE, resizeable);

			window = glfwCreateWindow(width, height, name.c_str(), fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);

			glfwSetWindowUserPointer(window, this);
			glfwSetWindowSizeCallback(window, Window::resized);
			glfwSetKeyCallback(window, Window::keyEvent);
			glfwSetCharCallback(window, Window::charEvent);
			glfwSetCursorPosCallback(window, Window::mouseMoveEvent);
			glfwSetMouseButtonCallback(window, Window::mouseButtonEvent);
			glfwSetScrollCallback(window, Window::mouseWheelEvent);

			createSurface();
			//createLogicalDevice();

			if (complete) {
				this->complete(width, height);
			}
		}

		void Window::complete(int width, int height) {
			createSwapChain(true, width, height);
			createImageViews();
			createDepthImage();
			createRenderPass();
			createFramebuffers();
			createSemafores();

			commandPools.resize(getNumFramebuffers());
			commandBuffers.resize(getNumFramebuffers());
			for (int i = 0; i < commandPools.size(); i++) {
				commandPools[i].init(Vulkan::getLogicalDevice(), this);
			}

			allWindows.add(this);
		}

		void Window::repaint() {

			if (cameras.length() > 0) {
				acquireNextImage();

				for (Camera *camera : cameras) {
					camera->prepare();
				}

				commandBuffers[currentImage].init(&commandPools[currentImage], true, true);
				commandBuffers[currentImage].begin();
				for (Camera *camera : cameras) {
					camera->capture(&commandBuffers[currentImage]);
				}
				commandBuffers[currentImage].end();
				submitBuffer(&commandBuffers[currentImage]);

				display();
			}
		}

		void Window::display() {
			List<VkSemaphore> semaphores;

			semaphores.add(renderFinishedSemaphores[currentImage]);


			VkPresentInfoKHR presentInfo = {};
			presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

			presentInfo.waitSemaphoreCount = 1;
			presentInfo.pWaitSemaphores = &renderFinishedSemaphores[currentImage];
			VkSwapchainKHR swapChains[] = { swapChain };
			presentInfo.swapchainCount = 1;
			presentInfo.pSwapchains = swapChains;
			presentInfo.pImageIndices = &currentImage;
			presentInfo.pResults = nullptr; // Optional

			VkResult result = vkQueuePresentKHR(Vulkan::getLogicalDevice()->getPresentQueue(), &presentInfo);

			if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
				onResized(swapChainExtent.width, swapChainExtent.height);
				Log::log("RECREATE SWAP CHAIN");
			} else if (result != VK_SUCCESS) {
				throw std::runtime_error("failed to present swap chain image!");
			}
		}

		void Window::submitBuffer(internal::CommandBuffer * cmd) {
		
			auto device = internal::Vulkan::getLogicalDevice();

			VkSubmitInfo submitInfo = {};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

			VkSemaphore waitSemaphores[] = { imageAvailableSemaphore };
			VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
			submitInfo.waitSemaphoreCount = 1;
			submitInfo.pWaitSemaphores = waitSemaphores;
			submitInfo.pWaitDstStageMask = waitStages;

			uint32_t img = getCurrentImage();

			VkCommandBuffer cmdBuffer = cmd->getBuffer();

			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &cmdBuffer;

			submitInfo.signalSemaphoreCount = 1;
			submitInfo.pSignalSemaphores = &renderFinishedSemaphores[img];

			vkResetFences(device->getDevice(), 1, &renderFinishedFences[img]);

			if (vkQueueSubmit(device->getGraphicsQueue(), 1, &submitInfo, renderFinishedFences[img]) != VK_SUCCESS) {
				throw std::runtime_error("failed to submit draw command buffer!");
			}
		}

		int Window::getWidth() {
			return swapChainExtent.width;
		}

		int Window::getHeight() {
			return swapChainExtent.height;
		}

		void Window::setCursorMode(CursorMode mode) {
			switch (mode) {
			case CursorMode::Normal: 
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				break;
			case CursorMode::Hidden:
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
				break;
			case CursorMode::Locked:
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				break;
			}
		}

		VkSemaphore Window::getImageSemaphore() {
			return imageAvailableSemaphore;
		}

		CommandPool * Window::getCommandPool() {
			return currentCommandPool;
		}

		void Window::acquireNextImage() {

			vkWaitForFences(Vulkan::getLogicalDevice()->getDevice(), 1, &renderFinishedFences[currentImage], VK_FALSE, 16000000L);
			
			VkResult result = vkAcquireNextImageKHR(Vulkan::getLogicalDevice()->getDevice(), swapChain, std::numeric_limits<uint64_t>::max(), imageAvailableSemaphore, VK_NULL_HANDLE, &currentImage);

			currentCommandPool = &commandPools[currentImage];
			currentCommandPool->clear();

			if (result == VK_ERROR_OUT_OF_DATE_KHR) {
				onResized(swapChainExtent.width, swapChainExtent.height);
				Log::log("RECREATE SWAP CHAIN");
				return;
			} else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
				throw std::runtime_error("failed to acquire swap chain image!");
			}
		}

		uint32_t Window::getCurrentImage() {
			return currentImage;
		}

		Window::~Window() {
			glfwDestroyWindow(window);

			allWindows.removeItem(this);

			for (int i = cameras.length() - 1; i >= 0; i--) {
				cameras[i]->setRenderWindow(nullptr);
			}
		}

		void Window::init() {
			glfwInit();
			glfwSetErrorCallback(Vulkan::windowErrorCallback);
		}

		VkSurfaceKHR Window::getSurface() {
			return surface;
		}

		VkSwapchainKHR Window::getSwapChain() {
			return swapChain;
		}

		VkFormat Window::getSwapChainImageFormat() {
			return swapChainImageFormat;
		}

		VkExtent2D Window::getSwapChainExtent() {
			return swapChainExtent;
		}

		VkFormat Window::getDepthFormat() {
			return Vulkan::findSupportedFormat(
				{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
				VK_IMAGE_TILING_OPTIMAL,
				VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
			);
		}

		int Window::getNumFramebuffers() {
			return framebuffers.size();
		}

		Framebuffer * Window::getFramebuffer(int i) {
			return &framebuffers[i];
		}

		CommandBuffer * Window::getCommandBuffer() {
			return &commandBuffers[currentImage];
		}

		Window *Window::getMainWindow() {
			return main;
		}

		bool Window::closeRequested() {
			return glfwWindowShouldClose(window);
		}

		void Window::pollEvents() {
			for (Window *window : allWindows) {
				window->inputHandler.update();
			}

			glfwPollEvents();

			for (int i = allWindows.length() - 1; i >= 0; i--) {
				if (allWindows[i]->closeRequested() && allWindows[i] != main) {
					vkDeviceWaitIdle(Vulkan::getLogicalDevice()->getDevice());
					delete allWindows[i];
				}
			}
		}

		void Window::createSwapChain(bool first, int width, int height) {
			LogicalDevice *device = Vulkan::getLogicalDevice();

			if (first) {
				swapChain = VReference<VkSwapchainKHR>(device->getDevice(), vkDestroySwapchainKHR);
			}

			const PhysicalDevice* physicalDevice = Vulkan::getPhysicalDevice();
			auto swapChainSupport = physicalDevice->getSwapChainSupportDetails(this);

			VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
			VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
			VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities, width, height);

			uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
			if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
				imageCount = swapChainSupport.capabilities.maxImageCount;
			}

			VkSwapchainCreateInfoKHR createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			createInfo.surface = surface;

			createInfo.minImageCount = imageCount;
			createInfo.imageFormat = surfaceFormat.format;
			createInfo.imageColorSpace = surfaceFormat.colorSpace;
			createInfo.imageExtent = extent;
			createInfo.imageArrayLayers = 1;
			createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

			auto indices = physicalDevice->getQueueFamilyIndices(this);
			uint32_t queueFamilyIndices[] = { (uint32_t)indices.graphicsFamily, (uint32_t)indices.presentFamily };

			if (indices.graphicsFamily != indices.presentFamily) {
				createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
				createInfo.queueFamilyIndexCount = 2;
				createInfo.pQueueFamilyIndices = queueFamilyIndices;
			} else {
				createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
				createInfo.queueFamilyIndexCount = 0; // Optional
				createInfo.pQueueFamilyIndices = nullptr; // Optional
			}

			createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
			createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

			createInfo.presentMode = presentMode;
			createInfo.clipped = VK_TRUE;
			createInfo.oldSwapchain = VK_NULL_HANDLE;

			VkSwapchainKHR oldSwapChain = swapChain;
			createInfo.oldSwapchain = oldSwapChain;

			VkSwapchainKHR newSwapChain;
			if (vkCreateSwapchainKHR(device->getDevice(), &createInfo, nullptr, &newSwapChain) != VK_SUCCESS) {
				throw std::runtime_error("failed to create swap chain!");
			}

			swapChain = newSwapChain;

			vkGetSwapchainImagesKHR(device->getDevice(), swapChain, &imageCount, nullptr);
			swapChainImages.resize(imageCount);
			vkGetSwapchainImagesKHR(device->getDevice(), swapChain, &imageCount, swapChainImages.data());

			swapChainImageFormat = surfaceFormat.format;
			swapChainExtent = extent;
		}

		void Window::createImageViews() {

			swapChainImageViews.resize(swapChainImages.size());

			for (uint32_t i = 0; i < swapChainImages.size(); i++) {
				swapChainImageViews[i].init(swapChainImages[i], swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
			}
		}

		void Window::createDepthImage() {
			VkFormat depthFormat = getDepthFormat();
			depthImage.init(swapChainExtent.width, swapChainExtent.height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_SAMPLE_COUNT_1_BIT);
			depthImage.createImageView(VK_IMAGE_ASPECT_DEPTH_BIT);

			depthImage.transition(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
		}

		void Window::createRenderPass() {
			renderPass.init(this);
		}

		void Window::createFramebuffers() {
			framebuffers.resize(swapChainImages.size());

			for (int i = 0; i < framebuffers.size(); i++) {
				framebuffers[i].init(swapChainExtent.width, swapChainExtent.height, &renderPass, &swapChainImageViews[i], depthImage.getImageView());
			}
		}

		void Window::createSemafores() {

			imageAvailableSemaphore.cleanup();

			LogicalDevice *device = Vulkan::getLogicalDevice();

			imageAvailableSemaphore = VReference<VkSemaphore>(device->getDevice(), vkDestroySemaphore);
			
			VkSemaphoreCreateInfo semaphoreInfo = {};
			semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

			if (vkCreateSemaphore(device->getDevice(), &semaphoreInfo, nullptr, imageAvailableSemaphore.replace()) != VK_SUCCESS) {
				throw std::runtime_error("failed to create semaphores!");
			}

			VkFenceCreateInfo fenceInfo = {};
			fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

			renderFinishedSemaphores.resize(getNumFramebuffers());
			renderFinishedFences.resize(getNumFramebuffers());

			for (int i = 0; i < getNumFramebuffers(); i++) {
				renderFinishedSemaphores[i].cleanup();
				renderFinishedFences[i].cleanup();

				renderFinishedSemaphores[i] = internal::VReference<VkSemaphore>(device->getDevice(), vkDestroySemaphore);
				renderFinishedFences[i] = internal::VReference<VkFence>(device->getDevice(), vkDestroyFence);

				if (vkCreateSemaphore(device->getDevice(), &semaphoreInfo, nullptr, renderFinishedSemaphores[i].replace()) != VK_SUCCESS) {
					throw std::runtime_error("failed to create semaphores!");
				}

				if (vkCreateFence(device->getDevice(), &fenceInfo, nullptr, renderFinishedFences[i].replace()) != VK_SUCCESS) {
					throw std::runtime_error("failed to create fence!");
				}
			}
		}

		void Window::onResized(int width, int height) {
			
			if (width > 0 && height > 0) {
				LogicalDevice *device = Vulkan::getLogicalDevice();

				vkDeviceWaitIdle(device->getDevice());

				createSwapChain(false, width, height);
				createImageViews();
				createDepthImage();
				createRenderPass();
				createFramebuffers();

				for (Camera *cam : cameras) {
					cam->windowResized();
				}
			}
		}

		VkSurfaceFormatKHR Window::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
			if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
				return{ VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
			}

			for (const auto& availableFormat : availableFormats) {
				if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
					return availableFormat;
				}
			}

			return availableFormats[0];
		}

		VkPresentModeKHR Window::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes) {
			for (const auto& availablePresentMode : availablePresentModes) {
				if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
					return availablePresentMode;
				}
			}

			return VK_PRESENT_MODE_FIFO_KHR;
		}

		VkExtent2D Window::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, int width, int height) {
			if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
				return capabilities.currentExtent;
			} else {
				VkExtent2D actualExtent = { width, height };

				actualExtent.width = FMath::max(capabilities.minImageExtent.width, FMath::min((int)capabilities.maxImageExtent.width, (int)actualExtent.width));
				actualExtent.height = FMath::max(capabilities.minImageExtent.height, FMath::min((int)capabilities.maxImageExtent.height, (int)actualExtent.height));

				return actualExtent;
			}
		}

		void Window::createSurface() {
			if (glfwCreateWindowSurface(Vulkan::getInstance(), window, nullptr, surface.replace()) != VK_SUCCESS) {
				throw std::runtime_error("failed to create window surface!");
			}
		}

		void Window::resized(GLFWwindow* window, int width, int height) {
			Window* w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
			w->onResized(width, height);
		}

		void Window::keyEvent(GLFWwindow * window, int key, int scancode, int action, int mods) {
			Window* w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
			if (action == GLFW_PRESS) {
				w->inputHandler.keyEvent(key, ButtonState::Down);
			} else if (action == GLFW_RELEASE) {
				w->inputHandler.keyEvent(key, ButtonState::Up);
			}
		}

		void Window::charEvent(GLFWwindow * window, unsigned int codepoint) {
			Window* w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
			w->inputHandler.charEvent(codepoint);
		}

		void Window::mouseMoveEvent(GLFWwindow * window, double x, double y) {
			Window* w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
			w->inputHandler.mouseMoveEvent(Vector2(x, w->getHeight() - y));
		}

		void Window::mouseButtonEvent(GLFWwindow * window, int button, int action, int mods) {
			Window* w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
			if (action == GLFW_PRESS) {
				w->inputHandler.mouseButtonEvent(button, ButtonState::Down);
			} else if (action == GLFW_RELEASE) {
				w->inputHandler.mouseButtonEvent(button, ButtonState::Up);
			}
		}

		void Window::mouseWheelEvent(GLFWwindow * window, double x, double y) {
			Window* w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
			w->inputHandler.mouseWheelEvent(y);
		}

		void Window::closeAll() {
			for (int i = allWindows.length() - 1; i >= 0; i--) {
				delete allWindows[i];
			}
		}

		List<Window *> Window::allWindows;
		Window *Window::main = nullptr;
	}
}