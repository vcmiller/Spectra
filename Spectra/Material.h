#pragma once

#include "RenderPass.h"
#include "Pipeline.h"
#include "Shader.h"
#include "Window.h"

namespace spectra {
	class Material {
	public:
		Material(internal::Window *window, Shader* shader);

	private:
		internal::RenderPass renderPass;
		internal::Pipeline pipeline;
	};
}