#pragma once

#include "Component.h"
#include "Color.h"
#include "CommandBuffer.h"
#include "Buffer.h"

namespace spectra {
	class MeshRenderer;
	class Spectra;
	namespace internal {
		class Pipeline;
	}

	class Light : public Component {
		friend class MeshRenderer;
		friend class Spectra;
		friend class internal::Pipeline;
	private:

		struct LightInfo {
			glm::vec4 position;
			glm::vec3 color;
			float intensity;
			float range;
		};

		void createInfoBuffer();
		void createDescriptorSet();

		void updateInfoBuffer();

		static void init();
		static void updateAll();
		static void cleanup();
		static void createDescriptorSetLayout();
		static void createDescriptorPool();


		internal::Buffer matrixBuffer;
		VkDescriptorSet descriptorSet;


		static internal::VReference<VkDescriptorSetLayout> descriptorLayout;
		static internal::VReference<VkDescriptorPool> descriptorPool;
		static List<Light*> allLights;

	public:
		Light();
		virtual ~Light();

		static int numLights();
		static Light* lightAt(int index);

		Color color;
		float intensity;
		float range;
		bool directional;
	};
}