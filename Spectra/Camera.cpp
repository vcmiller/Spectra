#include "Camera.h"
#include "Vulkan.h"
#include "LogicalDevice.h"

namespace spectra {
	Camera::Camera() {}

	Camera * Camera::currentCamera() {
		return current;
	}

	void Camera::onCreate() {
		window = internal::Window::getMainWindow();

		allCameras.add(this);

		commandBuffers.resize(window->getNumFramebuffers());
	}

	void Camera::onDestroy() {
		allCameras.remove(this);
	}

	internal::CommandBuffer * Camera::getCommandBuffer() {
		return currentCommandBuffer;
	}

	void Camera::capture() {
		current = this;

		int i = window->getCurrentImage();

		commandBuffers[i].init(true);
		currentCommandBuffer = &commandBuffers[i];

		begin(&commandBuffers[i], i);
		World::render();
		end(&commandBuffers[i]);

		current = nullptr;
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

		VkSemaphore signalSemaphores[] = { window->getRenderSemaphore() };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		if (vkQueueSubmit(device->getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
			throw std::runtime_error("failed to submit draw command buffer!");
		}
	}

	Camera *Camera::current = nullptr;
	ObjectSet<Camera> Camera::allCameras;
}