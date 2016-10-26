#pragma once
#include "Component.h"
#include "CommandBuffer.h"
#include "World.h"

#include <vector>

namespace spectra {
	class Spectra;

	namespace internal {
		class Window;
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
		friend class Spectra;
		friend class internal::Window;

		static Camera *current;

		void capture();

		void createSemafores();

		void begin(internal::CommandBuffer *cmd, int i);
		void end(internal::CommandBuffer *cmd);

		internal::Window *window = nullptr;

		bool pendingRender = false;

		internal::VReference<VkSemaphore> renderFinishedSemaphore;
		internal::VReference<VkFence> renderFinishedFence;

		std::vector<internal::CommandBuffer> commandBuffers;
		internal::CommandBuffer *currentCommandBuffer;
	};
}