#include "Scene.h"

namespace spectra {
	void Scene::depopulate() {
		for (GameObject *obj : objects) {
			obj->destroy();
		}
	}

	void Scene::add(GameObject *obj) {
		objects.add(obj);
	}
}
