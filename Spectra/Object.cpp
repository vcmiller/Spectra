#include "Object.h"

namespace spectra {
	Object::Object() { 
		static int objID;
		id = objID++;
	}

	std::string Object::getName() const {
		return name;
	}

	void Object::setName(std::string name) {
		this->name = name;
	}

	Object::SetPlacement::SetPlacement(void* set, int index) {
		this->set = set;
		this->index = index;
	}

	Object::SetPlacement::SetPlacement() {}
}
