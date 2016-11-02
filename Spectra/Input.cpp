#include "Input.h"
#include "Window.h"

namespace spectra {
	Input::Input() { }

	bool Input::getKey(int key) {
		return internal::Window::getMainWindow()->input.getKey(key);
	}

	bool Input::getKeyDown(int key) {
		return internal::Window::getMainWindow()->input.getKeyDown(key);
	}

	bool Input::getKeyUp(int key) {
		return internal::Window::getMainWindow()->input.getKeyUp(key);
	}

	std::string Input::getTypedText() {
		return internal::Window::getMainWindow()->input.getTypedText();
	}

	Vector2 Input::getMousePosition() {
		return internal::Window::getMainWindow()->input.getMousePosition();
	}

	Vector2 Input::getMouseDelta() {
		return internal::Window::getMainWindow()->input.getMouseDelta();
	}

	float Input::getMouseWheelDelta() {
		return internal::Window::getMainWindow()->input.getMouseWheelDelta();
	}

	bool Input::getMouseButton(int button) {
		return internal::Window::getMainWindow()->input.getMouseButton(button);
	}

	bool Input::getMouseButtonDown(int button) {
		return getMouseButtonDown(button);
	}

	bool Input::getMouseButtonUp(int button) {
		return getMouseButtonUp(button);
	}

	void Input::setCursorMode(CursorMode mode) {
		internal::Window::getMainWindow()->setCursorMode(mode);
	}

	
}