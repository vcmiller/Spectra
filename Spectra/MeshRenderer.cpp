#include "MeshRenderer.h"
#include "RenderPass.h"
#include "Pipeline.h"
#include "Camera.h"

namespace spectra {
	MeshRenderer::MeshRenderer() {}

	void MeshRenderer::init(Mesh * mesh, Material * material, internal::Window * window) {
		this->mesh = mesh;
		this->material = material;
		this->window = window;

		setRecievesRender(true);
	}

	void MeshRenderer::render() {
		material->updateUniformBuffer(&transform);

		internal::CommandBuffer *drawCmd = Camera::currentCamera()->getCommandBuffer();
		internal::Pipeline *pipeline = &material->pipeline;

		pipeline->bind(drawCmd);

		VkBuffer vertexBuffers[] = { mesh->getVertexBuffer()->getBuffer() };
		VkDeviceSize offsets[] = { 0 };

		vkCmdBindDescriptorSets(drawCmd->getBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->getLayout(), 0, 1, &material->descriptorSet, 0, nullptr);
		vkCmdBindVertexBuffers(drawCmd->getBuffer(), 0, 1, vertexBuffers, offsets);
		vkCmdBindIndexBuffer(drawCmd->getBuffer(), mesh->getIndexBuffer()->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
		vkCmdDrawIndexed(drawCmd->getBuffer(), mesh->indices.length(), 1, 0, 0, 0);
	}

	void MeshRenderer::createCommandBuffer() {
		auto device = internal::Vulkan::getLogicalDevice();

		drawCmd.init(false);
		drawCmd.begin();

		internal::Pipeline *pipeline = &material->pipeline;

		pipeline->bind(&drawCmd);

		VkBuffer vertexBuffers[] = { mesh->getVertexBuffer()->getBuffer() };
		VkDeviceSize offsets[] = { 0 };

		vkCmdBindDescriptorSets(drawCmd.getBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->getLayout(), 0, 1, &material->descriptorSet, 0, nullptr);
		vkCmdBindVertexBuffers(drawCmd.getBuffer(), 0, 1, vertexBuffers, offsets);
		vkCmdBindIndexBuffer(drawCmd.getBuffer(), mesh->getIndexBuffer()->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
		vkCmdDrawIndexed(drawCmd.getBuffer(), mesh->indices.length(), 1, 0, 0, 0);

		drawCmd.end();
	}
}