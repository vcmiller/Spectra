#include "MeshRenderer.h"
#include "RenderPass.h"
#include "Pipeline.h"

namespace spectra {
	MeshRenderer::MeshRenderer() {}

	void MeshRenderer::init(Mesh * mesh, Material * material, internal::Window * window) {
		this->mesh = mesh;
		this->material = material;
		this->window = window;

		createCommandBuffers();

		setRecievesRender(true);
	}

	void MeshRenderer::render() {
		auto device = internal::Vulkan::getLogicalDevice();
		material->updateUniformBuffer(&transform);

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { window->getImageSemaphore() };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		uint32_t img = window->getCurrentImage();

		VkCommandBuffer cmdBuffer = commandBuffers[img].getBuffer();

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &cmdBuffer;

		VkSemaphore signalSemaphores[] = { window->getRenderSemaphore() };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		if (vkQueueSubmit(device->getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
			throw std::runtime_error("failed to submit draw command buffer!");
		}
	}

	void MeshRenderer::createCommandBuffers() {

		auto device = internal::Vulkan::getLogicalDevice();

		commandBuffers.resize(window->getNumFramebuffers());

		for (size_t i = 0; i < commandBuffers.size(); i++) {
			commandBuffers[i].init(false);
			commandBuffers[i].begin();

			internal::RenderPass *renderPass = window->getRenderPass();
			internal::Pipeline *pipeline = &material->pipeline;

			internal::CommandBuffer *c = &commandBuffers[i];
			internal::Framebuffer *f = window->getFramebuffer(i);

			renderPass->begin(c, f);

			pipeline->bind(&commandBuffers[i]);

			VkBuffer vertexBuffers[] = { mesh->getVertexBuffer()->getBuffer() };
			VkDeviceSize offsets[] = { 0 };

			vkCmdBindDescriptorSets(commandBuffers[i].getBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->getLayout(), 0, 1, &material->descriptorSet, 0, nullptr);
			vkCmdBindVertexBuffers(commandBuffers[i].getBuffer(), 0, 1, vertexBuffers, offsets);
			vkCmdBindIndexBuffer(commandBuffers[i].getBuffer(), mesh->getIndexBuffer()->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
			vkCmdDrawIndexed(commandBuffers[i].getBuffer(), mesh->indices.length(), 1, 0, 0, 0);

			renderPass->end(&commandBuffers[i]);

			commandBuffers[i].end();
		}
	}
}