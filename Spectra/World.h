#pragma once

#include "List.h"
#include "ObjectSet.h"
#include "GameObject.h"
#include "ComponentBase.h"
#include "Scene.h"

namespace spectra {

	class World {
	public:
		static void update();
		static void preRender();
		static void render();

		static void clear();

		static void load(Scene* scene, bool replace = true);
		static void unload(std::string label);

	private:
		friend class GameObject;
		friend class Component;

		static ObjectSet<GameObject> gameObjects;
		static ObjectSet<Component> components;
		static ObjectSet<Component> updateComponents;
		static ObjectSet<Component> renderComponents;

		static List<GameObject*> newGameObjects;
		static List<Component*> newComponents;

		static List<GameObject*> deleteGameObjects;
		static List<Component*> deleteComponents;

		static List<Scene*> loadedScenes;

		World();
	};
}