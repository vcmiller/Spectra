#include <math.h>
#include <limits.h>
#include <glm/gtc/constants.hpp>

#include "Math.h"

namespace spectra {
	Math::Math() { }

	const float Math::pi = glm::pi<float>();
	const float Math::e = glm::e<float>();

	const float Math::infinity = std::numeric_limits<float>::infinity();
	const float Math::negativeInfinity = -infinity;
	const float Math::maxFloat = std::numeric_limits<float>::max();
	const float Math::minFlow = std::numeric_limits<float>::min();
	const int Math::maxInt = std::numeric_limits<int>::max();
	const int Math::minInt = std::numeric_limits<int>::min();

	const float Math::quarterCircle = pi / 2;
	const float Math::halfCircle = pi;
	const float Math::fullCircle = pi * 2;
	const float Math::toRadians = pi / 180.0f;
	const float Math::toDegrees = 180.0f / pi;

	float Math::sign(float f) {
		if (f < 0) {
			return -1.0f;
		} else {
			return 1.0f;
		}
	}

	int Math::nextPowerOfTwo(int v) {
		int p = 1;
		
		while (p < v) {
			p *= 2;
		}

		return p;
	}

	float Math::round(float f) {
		return std::roundf(f);
	}

	float Math::ceil(float f) {
		return std::ceilf(f);
	}

	float Math::floor(float f) {
		return std::floorf(f);
	}

	int Math::roundToInt(float f) {
		return int(std::roundf(f));
	}

	int Math::ceilToInt(float f) {
		return int(std::ceilf(f));
	}

	int Math::floorToInt(float f) {
		return int(std::floorf(f));
	}

	float Math::sin(float a) {
		return std::sinf(a);
	}

	float Math::cos(float a) {
		return std::cosf(a);
	}

	float Math::tan(float a) {
		return std::tanf(a);
	}

	float Math::asin(float s) {
		return std::asinf(s);
	}

	float Math::acos(float c) {
		return std::acosf(c);
	}

	float Math::atan(float t) {
		return std::atanf(t);
	}

	float Math::atan2(float y, float x) {
		return std::atan2f(y, x);
	}

	float Math::pow(float a, float b) {
		return std::powf(a, b);
	}

	float Math::exp(float a) {
		return std::expf(a);
	}

	float Math::log(float b, float v) {
		return std::logf(v) / std::logf(b);
	}

	float Math::ln(float a) {
		return std::logf(a);
	}

	float Math::log10(float a) {
		return std::log10f(a);
	}

	float Math::sqrt(float s) {
		return std::sqrtf(s);
	}

	float Math::min(float a, float b) {
		return std::fminf(a, b);
	}

	int Math::min(int a, int b) {
		return a > b ? b : a;
	}

	float Math::max(float a, float b) {
		return std::fmaxf(a, b);
	}

	int Math::max(int a, int b) {
		return a > b ? a : b;
	}

	float Math::abs(float f) {
		return std::fabsf(f);
	}

	int Math::abs(int i) {
		return std::abs(i);
	}

	float Math::clamp(float a, float b, float v) {
		if (v > b) {
			return b;
		} else if (v < a) {
			return a;
		} else {
			return v;
		}
	}

	int Math::clamp(int a, int b, int v) {
		if (v > b) {
			return b;
		} else if (v < a) {
			return a;
		} else {
			return v;
		}
	}

	float Math::clamp01(float v) {
		return clamp(v, 0.0f, 1.0f);
	}

	float Math::lerp(float a, float b, float amt) {
		return a + (b - a) * amt;
	}

	float Math::moveTowards(float a, float b, float amt) {
		if (abs(b - a) <= amt) {
			return b;
		} else if (b < a) {
			return a - amt;
		} else {
			return a + amt;
		}
	}

}