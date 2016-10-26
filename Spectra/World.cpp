#include "World.h"
#include "Log.h"
#include "MeshRenderer.h"

namespace spectra {

	spectra::ObjectSet<spectra::GameObject> World::gameObjects;
	spectra::ObjectSet<spectra::Component> World::components;
	spectra::ObjectSet<spectra::Component> World::updateComponents;
	spectra::ObjectSet<spectra::Component> World::renderComponents;
	spectra::List<GameObject*> World::newGameObjects;
	spectra::List<Component*> World::newComponents;
	spectra::List<GameObject*> World::deleteGameObjects;
	spectra::List<Component*> World::deleteComponents;
	spectra::List<Scene*> World::loadedScenes;

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

		deleteComponents.clear();
		deleteGameObjects.clear();
		newComponents.clear();
		newGameObjects.clear();
	}

	void World::preRender() {
		for (Component *cmp : renderComponents) {
			cmp->preRender();
		}
	}

	void World::render() {
		for (Component *cmp : renderComponents) {
			cmp->render();
		}
	}

	void World::clear() {
		for (Scene *scene : loadedScenes) {
			scene->depopulate();
			delete scene;
		}

		loadedScenes.clear();

		for (GameObject *object : gameObjects) {
			object->destroy();
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

		deleteComponents.clear();
		deleteGameObjects.clear();
	}

	void World::load(Scene *scene, bool replace) {
		if (replace) {
			clear();
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

