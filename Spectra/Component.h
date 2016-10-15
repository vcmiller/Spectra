#pragma once

#include "ComponentBase.h"
#include "GameObject.h"

namespace spectra {
	template<typename T> T *Component::getComponent() {
		getGameObject()->getComponent<T>();
	}

	template<typename T> List<T*> Component::getComponents() {
		return getGameObject()->getComponents<T>();
	}
}