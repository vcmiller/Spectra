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
		createMatrixBuffer();
		createDescriptorSet();
	}

	void Camera::onDestroy() {
		setRenderWindow(nullptr);
	}

	internal::CommandBuffer * Camera::getCommandBuffer() {
		return currentCommandBuffer;
	}

	void Camera::capture() {
		updateMatrixBuffer();

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

	void Camera::createMatrixBuffer() {
		VkDeviceSize bufferSize = sizeof(CameraMatrices);

		matrixBuffer.init(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	}

	void Camera::createDescriptorSet() {
		auto device = internal::Vulkan::getLogicalDevice();

		VkDescriptorSetLayout layouts[] = { descriptorLayout };
		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = descriptorPool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = layouts;

		if (vkAllocateDescriptorSets(device->getDevice(), &allocInfo, &descriptorSet) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate descriptor set!");
		}

		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = matrixBuffer.getBuffer();
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(CameraMatrices);

		VkWriteDescriptorSet descriptorWrite = {};

		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSet;
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bufferInfo;

		vkUpdateDescriptorSets(device->getDevice(), 1, &descriptorWrite, 0, nullptr);
	}

	void Camera::updateMatrixBuffer() {
		auto device = internal::Vulkan::getLogicalDevice();

		CameraMatrices ubo = {};

		ubo.view = transform.worldToLocalMatrix().mat;
		ubo.proj = projection.mat;

		void* data;
		vkMapMemory(device->getDevice(), matrixBuffer.getMemory(), 0, sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device->getDevice(), matrixBuffer.getMemory());
	}

	void Camera::init() {
		createDescriptorSetLayout();
		createDescriptorPool();
	}

	void Camera::cleanup() {
		descriptorLayout.cleanup();
		descriptorPool.cleanup();
	}

	void Camera::createDescriptorSetLayout() {
		internal::LogicalDevice *device = internal::Vulkan::getLogicalDevice();

		descriptorLayout.cleanup();

		descriptorLayout = internal::VReference<VkDescriptorSetLayout>(device->getDevice(), vkDestroyDescriptorSetLayout);

		VkDescriptorSetLayoutBinding descriptorLayoutBinding = {};
		descriptorLayoutBinding.binding = 0;
		descriptorLayoutBinding.descriptorCount = 1;
		descriptorLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorLayoutBinding.pImmutableSamplers = nullptr;
		descriptorLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

		VkDescriptorSetLayoutCreateInfo descriptorLayoutInfo = {};
		descriptorLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		descriptorLayoutInfo.bindingCount = 1;
		descriptorLayoutInfo.pBindings = &descriptorLayoutBinding;

		if (vkCreateDescriptorSetLayout(device->getDevice(), &descriptorLayoutInfo, nullptr, descriptorLayout.replace()) != VK_SUCCESS) {
			throw std::runtime_error("failed to create descriptor set layout!");
		}
	}

	void Camera::createDescriptorPool() {
		auto device = internal::Vulkan::getLogicalDevice();

		descriptorPool.cleanup();

		descriptorPool = internal::VReference<VkDescriptorPool>(device->getDevice(), vkDestroyDescriptorPool);

		VkDescriptorPoolSize descriptorPoolSize = {};
		descriptorPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorPoolSize.descriptorCount = 10;

		VkDescriptorPoolCreateInfo descriptorPoolInfo = {};
		descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptorPoolInfo.poolSizeCount = 1;
		descriptorPoolInfo.pPoolSizes = &descriptorPoolSize;
		descriptorPoolInfo.maxSets = 10;
		descriptorPoolInfo.pNext = nullptr;
		descriptorPoolInfo.flags = 0;

		if (vkCreateDescriptorPool(device->getDevice(), &descriptorPoolInfo, nullptr, descriptorPool.replace()) != VK_SUCCESS) {
			throw std::runtime_error("failed to create descriptor pool!");
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

	internal::VReference<VkDescriptorSetLayout> Camera::descriptorLayout;
	internal::VReference<VkDescriptorPool> Camera::descriptorPool;
}