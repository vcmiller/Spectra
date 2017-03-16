#define FLT_EVAL_METHOD 0
#include <cfloat>
#include <cmath>
#include <limits>
#include <glm/gtc/constants.hpp>

#include "FMath.h"

namespace spectra {
	FMath::FMath() { }

	const float FMath::pi = glm::pi<float>();
	const float FMath::e = glm::e<float>();

	const float FMath::infinity = std::numeric_limits<float>::infinity();
	const float FMath::negativeInfinity = -infinity;
	const float FMath::maxFloat = std::numeric_limits<float>::max();
	const float FMath::minFlow = std::numeric_limits<float>::min();
	const int FMath::maxInt = std::numeric_limits<int>::max();
	const int FMath::minInt = std::numeric_limits<int>::min();

	const float FMath::quarterCircle = pi / 2;
	const float FMath::halfCircle = pi;
	const float FMath::fullCircle = pi * 2;
	const float FMath::toRadians = pi / 180.0f;
	const float FMath::toDegrees = 180.0f / pi;

	float FMath::sign(float f) {
		if (f < 0) {
			return -1.0f;
		} else {
			return 1.0f;
		}
	}

	int FMath::nextPowerOfTwo(int v) {
		int p = 1;
		
		while (p < v) {
			p *= 2;
		}

		return p;
	}

	float FMath::round(float f) {
		return std::roundf(f);
	}

	float FMath::ceil(float f) {
		return std::ceilf(f);
	}

	float FMath::floor(float f) {
		return std::floorf(f);
	}

	int FMath::roundToInt(float f) {
		return int(std::roundf(f));
	}

	int FMath::ceilToInt(float f) {
		return int(std::ceilf(f));
	}

	int FMath::floorToInt(float f) {
		return int(std::floorf(f));
	}

	float FMath::sin(float a) {
		return std::sinf(a);
	}

	float FMath::cos(float a) {
		return std::cosf(a);
	}

	float FMath::tan(float a) {
		return std::tanf(a);
	}

	float FMath::asin(float s) {
		return std::asinf(s);
	}

	float FMath::acos(float c) {
		return std::acosf(c);
	}

	float FMath::atan(float t) {
		return std::atanf(t);
	}

	float FMath::atan2(float y, float x) {
		return std::atan2f(y, x);
	}

	float FMath::pow(float a, float b) {
		return std::powf(a, b);
	}

	float FMath::exp(float a) {
		return std::expf(a);
	}

	float FMath::log(float b, float v) {
		return std::logf(v) / std::logf(b);
	}

	float FMath::ln(float a) {
		return std::logf(a);
	}

	float FMath::log10(float a) {
		return std::log10f(a);
	}

	float FMath::sqrt(float s) {
		return std::sqrtf(s);
	}

	float FMath::min(float a, float b) {
		return std::fminf(a, b);
	}

	int FMath::min(int a, int b) {
		return a > b ? b : a;
	}

	float FMath::max(float a, float b) {
		return std::fmaxf(a, b);
	}

	int FMath::max(int a, int b) {
		return a > b ? a : b;
	}

	float FMath::abs(float f) {
		return std::fabsf(f);
	}

	int FMath::abs(int i) {
		return std::abs(i);
	}

	float FMath::clamp(float a, float b, float v) {
		if (v > b) {
			return b;
		} else if (v < a) {
			return a;
		} else {
			return v;
		}
	}

	int FMath::clamp(int a, int b, int v) {
		if (v > b) {
			return b;
		} else if (v < a) {
			return a;
		} else {
			return v;
		}
	}

	float FMath::clamp01(float v) {
		return clamp(v, 0.0f, 1.0f);
	}

	float FMath::lerp(float a, float b, float amt) {
		return a + (b - a) * amt;
	}

	float FMath::moveTowards(float a, float b, float amt) {
		if (abs(b - a) <= amt) {
			return b;
		} else if (b < a) {
			return a - amt;
		} else {
			return a + amt;
		}
	}

}