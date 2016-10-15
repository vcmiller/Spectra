#include "World.h"

namespace spectra {
	World::World() {}

	void World::update() {
		for (Component *component : updateComponents) {
			component->update();
		}

		for (Component *cmp : deleteComponents) {
			components.remove(cmp);
			updateComponents.remove(cmp);
			renderComponents.remove(cmp);

			cmp->gameObject.components.removeItem(cmp);

			delete cmp;
		}

		for (GameObject *obj : deleteGameObjects) {
			gameObjects.remove(obj);
			delete obj;
		}

		for (GameObject *obj : newGameObjects) {
			gameObjects.add(obj);
		}

		for (Component *cmp : newComponents) {
			components.add(cmp);
			if (cmp->receivesRender) {
				renderComponents.add(cmp);
			}

			if (cmp->receivesUpdate) {
				updateComponents.add(cmp);
			}
		}
	}

	void World::render() {
		for (Component *cmp : renderComponents) {
			cmp->render();
		}
	}

	void World::load(Scene *scene, bool replace) {
		if (replace) {
			for (Scene *scene : loadedScenes) {
				scene->depopulate();
				delete scene;
			}

			loadedScenes.clear();
		}

		if (loadedScenes.indexOf(scene) == -1) {
			loadedScenes.add(scene);
			scene->populate();
		}
	}

	void World::unload(std::string label) {
		for (int i = 0; i < loadedScenes.length(); i++) {
			Scene *scene = loadedScenes[i];
			if (scene->label == label) {
				scene->depopulate();
				loadedScenes.remove(i);
				delete scene;
				break;
			}
		}
	}
}

