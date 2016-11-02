#pragma once

#include "Vector.h"
#include "CursorMode.h"

#include <string>

namespace spectra {
	class Input {
	public:
		static bool getKey(int key);
		static bool getKeyDown(int key);
		static bool getKeyUp(int key);
		static std::string getTypedText();

		static Vector2 getMousePosition();
		static Vector2 getMouseDelta();
		static float getMouseWheelDelta();

		static bool getMouseButton(int button);
		static bool getMouseButtonDown(int button);
		static bool getMouseButtonUp(int button);

		static void setCursorMode(CursorMode mode);

	private:
		Input();
	};
}