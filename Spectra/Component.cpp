#include "Component.h"
#include "World.h"

namespace spectra {

	Component::Component() : gameObject(*assign), transform(assign->transform) {
		receivesRender = false;
		receivesUpdate = true;

		World::newComponents.add(this);
	}

	void Component::onCreate() { }
	void Component::update() { }
	void Component::render() { }
	void Component::onDestroy() { }

	void Component::destroy() {
		World::deleteComponents.add(this);
		onDestroy();
	}

	void Component::setRecievesUpdate(bool b) {
		this->receivesUpdate = b;
	}

	void Component::setRecievesRender(bool b) {
		this->receivesRender = b;
	}

	GameObject *Component::assign;
}