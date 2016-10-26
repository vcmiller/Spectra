#include "Camera.h"
#include "Vulkan.h"
#include "LogicalDevice.h"
#include "Window.h"

namespace spectra {
	Camera::Camera() {}

	void Camera::setRenderWindow(internal::Window * window) {
		if (this->window) {
			this->window->cameras.removeItem(this);
		}

		if (window) {
			window->cameras.add(this);
		}

		this->window = window;
	}

	internal::Window *Camera::getRenderWindow() {
		return window;
	}

	Camera * Camera::currentCamera() {
		return current;
	}

	void Camera::onCreate() {
		setRenderWindow(internal::Window::getMainWindow());

		//commandBuffers.resize(window->getNumFramebuffers());
		commandBuffers.resize(1);

		projection = Matrix4::perspective(glm::radians(45.0f), float(window->getWidth()) / float(window->getHeight()), 0.1f, 10.0f);

		createSemafores();
	}

	void Camera::onDestroy() {
		setRenderWindow(nullptr);
	}

	internal::CommandBuffer * Camera::getCommandBuffer() {
		return currentCommandBuffer;
	}

	void Camera::capture() {
		current = this;
		int i = window->getCurrentImage();

		World::preRender();

		commandBuffers[0].init(true);
		currentCommandBuffer = &commandBuffers[0];

		begin(&commandBuffers[0], i);
		World::render();
		end(&commandBuffers[0]);

		current = nullptr;

		pendingRender = true;
	}

	void Camera::createSemafores() {
		renderFinishedSemaphore.cleanup();
		renderFinishedFence.cleanup();

		internal::LogicalDevice *device = internal::Vulkan::getLogicalDevice();

		renderFinishedSemaphore = internal::VReference<VkSemaphore>(device->getDevice(), vkDestroySemaphore);
		renderFinishedFence = internal::VReference<VkFence>(device->getDevice(), vkDestroyFence);

		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		if (vkCreateSemaphore(device->getDevice(), &semaphoreInfo, nullptr, renderFinishedSemaphore.replace()) != VK_SUCCESS) {
			throw std::runtime_error("failed to create semaphores!");
		}

		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		if (vkCreateFence(device->getDevice(), &fenceInfo, nullptr, renderFinishedFence.replace()) != VK_SUCCESS) {
			throw std::runtime_error("failed to create fence!");
		}
	}

	void Camera::begin(internal::CommandBuffer *cmd, int i) {
		cmd->begin();
		window->getRenderPass()->begin(cmd, window->getFramebuffer(i));
	}

	void Camera::end(internal::CommandBuffer *cmd) {
		window->getRenderPass()->end(cmd);
		cmd->end();

		auto device = internal::Vulkan::getLogicalDevice();

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { window->getImageSemaphore() };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		uint32_t img = window->getCurrentImage();

		VkCommandBuffer cmdBuffer = cmd->getBuffer();

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &cmdBuffer;

		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &renderFinishedSemaphore;

		vkResetFences(device->getDevice(), 1, &renderFinishedFence);

		if (vkQueueSubmit(device->getGraphicsQueue(), 1, &submitInfo, renderFinishedFence) != VK_SUCCESS) {
			throw std::runtime_error("failed to submit draw command buffer!");
		}
	}

	Camera *Camera::current = nullptr;
}