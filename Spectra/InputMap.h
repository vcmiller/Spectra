#pragma once

#include "Map.h"
#include "ButtonState.h"
#include "Key.h"

#include <vector>

namespace spectra {
	namespace internal {
		class InputMap {

		public:
			InputMap(int size);

			void update();

			void press(int key);
			void release(int key);

			bool getDown(int key) const;
			bool getUp(int key) const;
			bool get(int key) const;

			ButtonState getState(int key);

		private:
			std::vector<ButtonState> keys;
		};
	}
}