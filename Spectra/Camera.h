#pragma once
#include "Component.h"
#include "Window.h"
#include "CommandBuffer.h"
#include "World.h"

#include <vector>

namespace spectra {
	class Spectra;
	class Camera : public Component {
	public:
		Camera();

		void setRenderWindow(internal::Window *window);

		static Camera* currentCamera();

		void onCreate();
		void onDestroy();

		internal::CommandBuffer *getCommandBuffer();

	private:
		friend class Spectra;

		static Camera *current;
		static ObjectSet<Camera> allCameras;

		void capture();

		void begin(internal::CommandBuffer *cmd, int i);
		void end(internal::CommandBuffer *cmd);

		internal::Window *window;

		std::vector<internal::CommandBuffer> commandBuffers;
		internal::CommandBuffer *currentCommandBuffer;
	};
}