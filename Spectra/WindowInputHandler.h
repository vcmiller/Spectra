#pragma once

#include "InputMap.h"
#include "ButtonState.h"
#include "Vector.h"

#include <string>

namespace spectra {
	namespace internal {

		class WindowInputHandler {
		public:
			WindowInputHandler();

			void keyEvent(int key, ButtonState action);
			void charEvent(unsigned int codepoint);
			void mouseMoveEvent(Vector2 position);
			void mouseButtonEvent(int button, ButtonState action);
			void mouseWheelEvent(float delta);

			void update();

			bool getKey(int key) const;
			bool getKeyUp(int key) const;
			bool getKeyDown(int key) const;
			std::string getTypedText() const;

			Vector2 getMousePosition() const;
			Vector2 getMouseDelta() const;
			float getMouseWheelDelta() const;

			bool getMouseButton(int button) const;
			bool getMouseButtonDown(int button) const;
			bool getMouseButtonUp(int button) const;

		private:
			InputMap keys{ 400 };
			InputMap mouseButtons{ 6 };

			std::string typedText;
			Vector2 mousePosition;
			Vector2 mouseDelta;
			float mouseWheelDelta;

			WindowInputHandler(const WindowInputHandler&);
			void operator=(const WindowInputHandler &);
		};
	}
}