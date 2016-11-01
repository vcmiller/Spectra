#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VReference.h"
#include "Window.h"
#include "Shader.h"
#include "RenderPass.h"
#include "CommandBuffer.h"

namespace spectra {
	namespace internal {
		class Pipeline {
		public:
			Pipeline();

			void init(Camera *camera, Shader *shader, RenderPass *renderPass);
			void bind(CommandBuffer *commandBuffer);

			VkPipelineLayout getLayout();
			VkPipeline getPipeline();

			bool isInitialized();
			bool outOfDate();

		private:
			bool initialized = false;

			Camera *camera;

			VReference<VkPipelineLayout> pipelineLayout;
			VReference<VkPipeline> graphicsPipeline;

			int renderPassVersion = -1;
		};
	}
}