#pragma once
#include "GameObject.h"
#include "List.h"

#include <string>

namespace spectra {
	class Scene {
	public:
		virtual void populate() = 0;

		virtual void depopulate();
		void add(GameObject* obj);

		std::string label;

	private:
		List<GameObject*> objects;
	};
}