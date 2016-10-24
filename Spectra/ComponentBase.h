#pragma once

#include "Object.h"
#include "Transform.h"

namespace spectra {
	class GameObject;

	class Component : public Object {
	public:

		virtual void onCreate();
		virtual void update();
		virtual void render();
		virtual void onDestroy();

		void destroy();

		GameObject &gameObject;
		Transform &transform;

		template <class T> T *getComponent();
		template <class T> List<T*> getComponents();

		void setRecievesUpdate(bool b);
		void setRecievesRender(bool b);

	private:
		friend class World;
		friend class GameObject;
		
		bool receivesUpdate;
		bool receivesRender;

		static GameObject *assign;

	protected:
		Component();
	};
}