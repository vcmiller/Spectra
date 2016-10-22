#include "Material.h"

namespace spectra {
	Material::Material(internal::Window *window, Shader * shader) {
		renderPass.init(window);
		pipeline.init(window, shader, &renderPass);
	}
}