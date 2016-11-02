#include "Camera.h"
#include "Vulkan.h"
#include "LogicalDevice.h"
#include "Window.h"

namespace spectra {
	Camera::Camera() {}

	void Camera::setRenderWindow(internal::Window * window) {
		if (window != this->window) {
			if (this->window) {
				this->window->cameras.removeItem(this);
			}

			if (window) {
				window->cameras.add(this);
			}

			this->window = window;
		}
	}

	internal::Window *Camera::getRenderWindow() {
		return window;
	}

	internal::RenderPass *Camera::getRenderPass() {
		return &renderPass;
	}

	int Camera::getRenderPassVersion() {
		return renderPassVersion;
	}

	Camera * Camera::currentCamera() {
		return current;
	}

	void Camera::onCreate() {
		setRenderWindow(internal::Window::getMainWindow());

		setProjection(45, .1f, 100.0f);

		createMatrixBuffer();
		createDescriptorSet();
		createRenderPass();
	}

	void Camera::onDestroy() {
		setRenderWindow(nullptr);
	}

	void Camera::setProjection(float fov, float clipNear, float clipFar) {
		this->fov = fov;
		this->nearClip = clipNear;
		this->farClip = clipFar;

		float w = window->getWidth() * viewWidth;
		float h = window->getHeight() * viewHeight;

		projection = Matrix4::perspective(glm::radians(fov), w / h, clipNear, clipFar);
	}

	void Camera::setViewport(float x, float y, float width, float height) {
		viewX = x;
		viewY = y;
		viewWidth = width;
		viewHeight = height;

		setProjection(fov, nearClip, farClip);
	}

	void Camera::setClearFlags(bool clearColor, bool clearDepth) {
		this->clearColor = clearColor;
		this->clearDepth = clearDepth;

		createRenderPass();
	}

	void Camera::setBackgroundColor(const Color & color) {
		this->backgroundColor = color;
	}

	void Camera::prepare() {
		updateMatrixBuffer();

		current = this;

		World::preRender();
	}

	void Camera::capture(internal::CommandBuffer *cmd) {

		current = this;
		int i = window->getCurrentImage();

		begin(cmd, i);
		World::render();
		end(cmd);

		current = nullptr;
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

	void Camera::createRenderPass() {
		renderPassVersion++;
		renderPass.init(window, clearColor, clearDepth);
	}

	void Camera::windowResized() {
		createRenderPass();

		setProjection(fov, nearClip, farClip);
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
		int x = Math::roundToInt(window->getWidth() * viewX);
		int y = Math::roundToInt(window->getHeight() * (1.0f - viewY - viewHeight));
		int w = Math::roundToInt(window->getWidth() * viewWidth);
		int h = Math::roundToInt(window->getHeight() * viewHeight);

		renderPass.begin(cmd, window->getFramebuffer(i), x, y, w, h, backgroundColor);

		VkViewport viewport = {};
		viewport.x = x;
		viewport.y = y;
		viewport.width = w;
		viewport.height = h;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		vkCmdSetViewport(cmd->getBuffer(), 0, 1, &viewport);
	}

	void Camera::end(internal::CommandBuffer *cmd) {
		renderPass.end(cmd);
	}

	Camera *Camera::current = nullptr;

	internal::VReference<VkDescriptorSetLayout> Camera::descriptorLayout;
	internal::VReference<VkDescriptorPool> Camera::descriptorPool;
}