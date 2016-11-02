#include "InputMap.h"

namespace spectra {
	namespace internal {
		InputMap::InputMap(int size) {
			keys.resize(size, ButtonState::None);
		}
		
		void InputMap::update() {
			for (int i = 0; i < keys.size(); i++) {
				if (keys[i] == ButtonState::Down) {
					keys[i] = ButtonState::Held;
				} else if (keys[i] == ButtonState::Up) {
					keys[i] = ButtonState::None;
				}
			}
		}
		
		void InputMap::press(int key) {
			if (keys.size() <= key) {
				keys.resize(key + 1, ButtonState::None);
			}

			keys[key] = ButtonState::Down;
		}
		
		void InputMap::release(int key) {
			if (keys.size() <= key) {
				keys.resize(key + 1, ButtonState::None);
			}

			keys[key] = ButtonState::Up;
		}
		
		bool InputMap::getDown(int key) const {
			if (keys.size() > key) {
				return keys[key] == ButtonState::Down;
			} else {
				return false;
			}
		}
		
		bool InputMap::getUp(int key) const {
			if (keys.size() > key) {
				return keys[key] == ButtonState::Up;
			} else {
				return false;
			}
		}
		
		bool InputMap::get(int key) const {
			if (keys.size() > key) {
				ButtonState state = keys[key];
				return state == ButtonState::Down || state == ButtonState::Held;
			} else {
				return false;
			}
		}
		
		ButtonState InputMap::getState(int key) {
			if (keys.size() >= key) {
				return keys[key];
			} else {
				return ButtonState::None;
			}
		}
	}
}
