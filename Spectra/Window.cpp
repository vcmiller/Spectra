#include "Window.h"
#include "Math.h"
#include "Vulkan.h"
#include "PhysicalDevice.h"

namespace spectra {
	namespace internal {
		Window::Window(int width, int height, std::string name, bool resizeable, bool complete) : surface(Vulkan::getInstance(), vkDestroySurfaceKHR) {
			this->width = width;
			this->height = height;

			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			glfwWindowHint(GLFW_RESIZABLE, resizeable);

			window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);

			glfwSetWindowUserPointer(window, this);
			glfwSetWindowSizeCallback(window, Window::resized);

			createSurface();
			//createLogicalDevice();

			if (complete) {
				this->complete();
			}
		}

		void Window::complete() {
			createSwapChain();
			createImageViews();
			createDepthImage();
			createRenderPass();
			createFramebuffers();
			createSemafores();
		}

		void Window::display() {
			VkSemaphore semaphores[] = { renderFinishedSemaphore };

			VkPresentInfoKHR presentInfo = {};
			presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

			presentInfo.waitSemaphoreCount = 1;
			presentInfo.pWaitSemaphores = semaphores;
			VkSwapchainKHR swapChains[] = { swapChain };
			presentInfo.swapchainCount = 1;
			presentInfo.pSwapchains = swapChains;
			presentInfo.pImageIndices = &currentImage;
			presentInfo.pResults = nullptr; // Optional

			VkResult result = vkQueuePresentKHR(Vulkan::getLogicalDevice()->getPresentQueue(), &presentInfo);

			if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
				recreateSwapChain();
				Log::log("RECREATE SWAP CHAIN");
			} else if (result != VK_SUCCESS) {
				throw std::runtime_error("failed to present swap chain image!");
			}
		}

		VkSemaphore Window::getImageSemaphore() {
			return imageAvailableSemaphore;
		}

		VkSemaphore Window::getRenderSemaphore() {
			return renderFinishedSemaphore;
		}

		void Window::acquireNextImage() {
			VkResult result = vkAcquireNextImageKHR(Vulkan::getLogicalDevice()->getDevice(), swapChain, std::numeric_limits<uint64_t>::max(), imageAvailableSemaphore, VK_NULL_HANDLE, &currentImage);

			if (result == VK_ERROR_OUT_OF_DATE_KHR) {
				recreateSwapChain();
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

		RenderPass *Window::getRenderPass() {
			return &renderPass;
		}

		int Window::getNumFramebuffers() {
			return framebuffers.size();
		}

		Framebuffer * Window::getFramebuffer(int i) {
			return &framebuffers[i];
		}

		Window *Window::getMainWindow() {
			return main;
		}

		bool Window::closeRequested() {
			return glfwWindowShouldClose(window);
		}

		void Window::pollEvents() {
			glfwPollEvents();
		}

		void Window::createSwapChain() {
			LogicalDevice *device = Vulkan::getLogicalDevice();

			swapChain = VReference<VkSwapchainKHR>(device->getDevice(), vkDestroySwapchainKHR);

			const PhysicalDevice* physicalDevice = Vulkan::getPhysicalDevice();
			auto swapChainSupport = physicalDevice->getSwapChainSupportDetails(this);

			VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
			VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
			VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

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
			renderFinishedSemaphore.cleanup();

			LogicalDevice *device = Vulkan::getLogicalDevice();

			imageAvailableSemaphore = VReference<VkSemaphore>(device->getDevice(), vkDestroySemaphore);
			renderFinishedSemaphore = VReference<VkSemaphore>(device->getDevice(), vkDestroySemaphore);

			VkSemaphoreCreateInfo semaphoreInfo = {};
			semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

			if (vkCreateSemaphore(device->getDevice(), &semaphoreInfo, nullptr, imageAvailableSemaphore.replace()) != VK_SUCCESS ||
				vkCreateSemaphore(device->getDevice(), &semaphoreInfo, nullptr, renderFinishedSemaphore.replace()) != VK_SUCCESS) {

				throw std::runtime_error("failed to create semaphores!");
			}
		}

		void Window::recreateSwapChain() {
			LogicalDevice *device = Vulkan::getLogicalDevice();

			vkDeviceWaitIdle(device->getDevice());

			createSwapChain();
			createImageViews();
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

		VkExtent2D Window::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
			if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
				return capabilities.currentExtent;
			} else {
				VkExtent2D actualExtent = { width, height };

				actualExtent.width = Math::max(capabilities.minImageExtent.width, Math::min((int)capabilities.maxImageExtent.width, (int)actualExtent.width));
				actualExtent.height = Math::max(capabilities.minImageExtent.height, Math::min((int)capabilities.maxImageExtent.height, (int)actualExtent.height));

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
			w->recreateSwapChain();
		}


		Window *Window::main = nullptr;
	}
}