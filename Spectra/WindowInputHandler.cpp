#include "WindowInputHandler.h"

namespace spectra {
	namespace internal {
		WindowInputHandler::WindowInputHandler() { }
		WindowInputHandler::WindowInputHandler(const WindowInputHandler &) {}
		void WindowInputHandler::operator=(const WindowInputHandler &) {}

		void WindowInputHandler::keyEvent(int key, ButtonState action) {
			if (action == ButtonState::Down) {
				keys.press(key);
			} else if (action == ButtonState::Up) {
				keys.release(key);
			}
		}

		void WindowInputHandler::charEvent(unsigned int codepoint) {
			typedText += codepoint;
		}

		void WindowInputHandler::mouseMoveEvent(Vector2 position) {
			mouseDelta = position - mousePosition;
			mousePosition = position;
		}

		void WindowInputHandler::mouseButtonEvent(int button, ButtonState action) {
			if (action == ButtonState::Down) {
				mouseButtons.press(button);
			} else if (action == ButtonState::Up) {
				mouseButtons.release(button);
			}
		}

		void WindowInputHandler::mouseWheelEvent(float delta) {
			mouseWheelDelta = delta;
		}

		void WindowInputHandler::update() {
			keys.update();
			mouseButtons.update();
			typedText = "";
			mouseDelta = Vector2(0, 0);
			mouseWheelDelta = 0;
		}

		bool WindowInputHandler::getKey(int key) const {
			return keys.get(key);
		}

		bool WindowInputHandler::getKeyUp(int key) const {
			return keys.getUp(key);
		}

		bool WindowInputHandler::getKeyDown(int key) const {
			return keys.getDown(key);
		}

		std::string WindowInputHandler::getTypedText() const {
			return typedText;
		}

		Vector2 WindowInputHandler::getMousePosition() const {
			return mousePosition;
		}

		Vector2 WindowInputHandler::getMouseDelta() const {
			return mouseDelta;
		}

		float WindowInputHandler::getMouseWheelDelta() const {
			return mouseWheelDelta;
		}

		bool WindowInputHandler::getMouseButton(int button) const {
			return mouseButtons.get(button);
		}

		bool WindowInputHandler::getMouseButtonDown(int button) const {
			return mouseButtons.getDown(button);
		}

		bool WindowInputHandler::getMouseButtonUp(int button) const {
			return mouseButtons.getUp(button);
		}

	}
}