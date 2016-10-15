#include "Component.h"
#include "World.h"

namespace spectra {

	Component::Component(GameObject &obj) : gameObject(obj), transform(obj.transform) {
		receivesRender = false;
		receivesUpdate = true;
	}

	void Component::onCreate() { }
	void Component::update() { }
	void Component::render() { }
	void Component::onDestroy() { }

	void Component::destroy() {
		World::deleteComponents.add(this);
		onDestroy();
	}
}