#pragma once
#include "Component.h"
#include "CommandBuffer.h"
#include "World.h"
#include "Buffer.h"

#include <vector>

namespace spectra {
	class Spectra;

	namespace internal {
		class Window;
		class Pipeline;
	}

	class Camera : public Component {
	public:
		Camera();

		void setRenderWindow(internal::Window *window);
		internal::Window *getRenderWindow();

		static Camera* currentCamera();

		void onCreate();
		void onDestroy();

		internal::CommandBuffer *getCommandBuffer();

		Matrix4 projection;

	private:
		struct CameraMatrices {
			glm::mat4 view;
			glm::mat4 proj;
		};

		friend class Spectra;
		friend class MeshRenderer;
		friend class internal::Window;
		friend class internal::Pipeline;

		static Camera *current;

		void capture();

		void createSemafores();

		void createMatrixBuffer();
		void createDescriptorSet();
		void updateMatrixBuffer();

		static void init();
		static void cleanup();
		static void createDescriptorSetLayout();
		static void createDescriptorPool();

		void begin(internal::CommandBuffer *cmd, int i);
		void end(internal::CommandBuffer *cmd);

		internal::Window *window = nullptr;

		bool pendingRender = false;

		internal::Buffer matrixBuffer;
		VkDescriptorSet descriptorSet;

		internal::VReference<VkSemaphore> renderFinishedSemaphore;
		internal::VReference<VkFence> renderFinishedFence;

		std::vector<internal::CommandBuffer> commandBuffers;
		internal::CommandBuffer *currentCommandBuffer;

		static internal::VReference<VkDescriptorSetLayout> descriptorLayout;
		static internal::VReference<VkDescriptorPool> descriptorPool;
	};
}