#include "MeshRenderer.h"
#include "RenderPass.h"
#include "Pipeline.h"
#include "Camera.h"
#include "Shader.h"

#include <chrono>

namespace spectra {
	MeshRenderer::MeshRenderer() {}

	MeshRenderer::~MeshRenderer() { }

	void MeshRenderer::init(Mesh * mesh, Material * material) {
		this->mesh = mesh;
		this->material = material;

		setRecievesRender(true);
		createMatrixBuffer();
		createDescriptorSet();
	}

	void MeshRenderer::render(int pass) {
		updateMatrixBuffer();

		internal::CommandBuffer *drawCmd = Camera::currentCamera()->getRenderWindow()->getCommandBuffer();
		internal::Pipeline *pipeline = material->getShader()->getPipeline(Camera::currentCamera(), pass);

		pipeline->bind(drawCmd);

		VkBuffer vertexBuffers[] = { mesh->getVertexBuffer()->getBuffer() };
		VkDeviceSize offsets[] = { 0 };

		VkDescriptorSet descriptorSets[] = { Camera::currentCamera()->descriptorSet, descriptorSet, material->descriptorSet };

		vkCmdBindDescriptorSets(drawCmd->getBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->getLayout(), 0, 3, descriptorSets, 0, nullptr);
		vkCmdBindVertexBuffers(drawCmd->getBuffer(), 0, 1, vertexBuffers, offsets);
		vkCmdBindIndexBuffer(drawCmd->getBuffer(), mesh->getIndexBuffer()->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
		vkCmdDrawIndexed(drawCmd->getBuffer(), mesh->indices.length(), 1, 0, 0, 0);
	}

	void MeshRenderer::preRender(int pass) {
		material->check(Camera::currentCamera(), pass);
	}

	void MeshRenderer::createMatrixBuffer() {
		VkDeviceSize bufferSize = sizeof(ObjectMatrices);

		matrixBuffer.init(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	}

	void MeshRenderer::createDescriptorSet() {
		auto device = internal::Vulkan::getLogicalDevice();

		VkDescriptorSetLayout layouts[] = { material->shader->matricesLayout };
		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = material->shader->matricesPool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = layouts;

		if (vkAllocateDescriptorSets(device->getDevice(), &allocInfo, &descriptorSet) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate descriptor set!");
		}

		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = matrixBuffer.getBuffer();
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(ObjectMatrices);

		VkWriteDescriptorSet descriptorWrite = {};

		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSet;
		descriptorWrite.dstBinding = 1;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bufferInfo;

		vkUpdateDescriptorSets(device->getDevice(), 1, &descriptorWrite, 0, nullptr);
	}

	void MeshRenderer::updateMatrixBuffer() {

		auto device = internal::Vulkan::getLogicalDevice();

		ObjectMatrices ubo = {};

		Matrix4 mat = transform.localToWorldMatrix();
		ubo.model = mat.mat;

		void* data;
		vkMapMemory(device->getDevice(), matrixBuffer.getMemory(), 0, sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device->getDevice(), matrixBuffer.getMemory());
	}
}