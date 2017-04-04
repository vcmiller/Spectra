#pragma once
#include "Component.h"
#include "CommandBuffer.h"
#include "World.h"
#include "Buffer.h"
#include "RenderPass.h"
#include "Material.h"

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
		internal::RenderPass *getRenderPass();

		int getRenderPassVersion();

		static Camera* currentCamera();

		void onCreate();
		void onDestroy();

		void setProjection(float fov, float clipNear, float clipFar);
		void setViewport(float x, float y, float width, float height);
		void setClearFlags(bool clearColor, bool clearDepth);
		void setBackgroundColor(const Color &color);

	private:
		struct CameraMatrices {
			glm::mat4 view;
			glm::mat4 proj;
			glm::vec3 pos;
		};

		Matrix4 projection;

		friend class Spectra;
		friend class MeshRenderer;
		friend class internal::Window;
		friend class internal::Pipeline;

		static Camera *current;

		void createMatrixBuffer();
		void createDescriptorSet();
		void createRenderPass();

		void windowResized();

		void updateMatrixBuffer();

		static void init();
		static void cleanup();
		static void createDescriptorSetLayout();
		static void createDescriptorPool();

		void prepare();
		void capture(internal::CommandBuffer *cmd);

		void begin(internal::CommandBuffer *cmd, int i);
		void end(internal::CommandBuffer *cmd);

		internal::Window *window = nullptr;
		internal::RenderPass renderPass;

		internal::Buffer matrixBuffer;
		VkDescriptorSet descriptorSet;

		int renderPassVersion = -1;

		float fov = 45;
		float farClip = 100;
		float nearClip = 0.1f;

		bool clearColor = true;
		bool clearDepth = true;

		float viewX = 0.0f;
		float viewY = 0.0f;
		float viewWidth = 1.0f;
		float viewHeight = 1.0f;

		Color backgroundColor = Color(0, 0, 0, 1);

		static internal::VReference<VkDescriptorSetLayout> descriptorLayout;
		static internal::VReference<VkDescriptorPool> descriptorPool;
	};
}