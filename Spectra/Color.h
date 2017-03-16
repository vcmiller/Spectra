#pragma once

namespace spectra {
	class Color {
	public:
		float r, g, b, a;
		Color(float r, float g, float b, float a = 1.0f);
		Color();
	};
}