#pragma once

#include "ComponentBase.h"
#include "GameObject.h"

namespace spectra {
	template<typename T> T *Component::getComponent() {
		return gameObject.getComponent<T>();
	}

	template<typename T> List<T*> Component::getComponents() {
		return gameObject.getComponents<T>();
	}
}