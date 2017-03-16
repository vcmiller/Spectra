#include "Color.h"

namespace spectra {
	Color::Color(float r, float g, float b, float a) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
	Color::Color() : Color(0, 0, 0, 0) {}
}