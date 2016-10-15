#pragma once

#include "Object.h"
#include "Transform.h"
#include "ComponentBase.h"

namespace spectra {
	class Component;

	class GameObject : public Object {
	public:
		Transform transform;

		GameObject();

		template <class T> void addComponent() {
			T* component = new T();
			component->gameObject = this;
			components.add(component);
			component->onCreate();
		}

		template <class T> T *getComponent() {
			for (Component *component : components) {
				T* t = dynamic_cast<T*>(component);
				if (t) {
					return t;
				}
			}

			return nullptr;
		}

		template <class T> List<T*> getComponents() {
			List<T*> found;

			for (Component *component : components) {
				T* t = dynamic_cast<T*>(component);
				if (t) {
					found.add(t);
				}
			}

			return found;
		}

		void destroy();
	
	private:
		friend class Component;
		friend class World;
		List<Component*> components;
	};
}