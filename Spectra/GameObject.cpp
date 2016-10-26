#include "GameObject.h"
#include "World.h"
#include "Log.h"

namespace spectra {
	GameObject::GameObject() {
		World::newGameObjects.add(this);
	}

	void GameObject::destroy() {
		World::deleteGameObjects.add(this);

		for (Component *c : components) {
			c->destroy();
		}
	}
}
