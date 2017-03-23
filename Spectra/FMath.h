#pragma once

namespace spectra {
	// Utility class for handling math operations.
	class FMath {
	private:
		// Private constructor, no instances.
		FMath();

	public:
		// The constant pi, equal to approximately 3.141592.
		static const float pi;

		// The constant e, equal to approximately 2.71828.
		static const float e;

		// Positive infinity.
		static const float infinity;

		// Negative infinity.
		static const float negativeInfinity;

		// Max value of a float.
		static const float maxFloat;

		// Min value of a float.
		static const float minFlow;

		// Max value of an int.
		static const int maxInt;

		// Min value of an int.
		static const int minInt;

		// A quarter of a revolution in radians, equal to pi / 2.
		static const float quarterCircle;

		// A half of a revolution in radians, equal to pi.
		static const float halfCircle;

		// A full revolution in radians, equal to 2 * pi.
		static const float fullCircle;

		// Convert radians to degrees.
		static const float toRadians;

		// Convert degrees to radians.
		static const float toDegrees;

		// Compute sign value (-1 or 1).
		static float sign(float f);

		// Compute smallest power of two that is greater than v.
		static int nextPowerOfTwo(int v);

		// Round a float to the nearest whole number.
		static float round(float f);

		// Round up to nearest whole number greater than f.
		static float ceil(float f);

		// Round down to nearest whole number less than f.
		static float floor(float f);

		// Round a float to the nearest integer.
		static int roundToInt(float f);

		// Round up to nearest integer greater than f.
		static int ceilToInt(float f);

		// Round down to nearest integer less than f.
		static int floorToInt(float f);

		// Compute sine of angle (in radians).
		static float sin(float a);

		// Compute cosine of angle (in radians).
		static float cos(float a);

		// Compute tangent of angle (in radians).
		static float tan(float a);

		// Compute arcsine in radians.
		static float asin(float s);

		// Compute arccosine in radians.
		static float acos(float c);

		// Compute arctangent in radians.
		static float atan(float t);

		// Compute arctangent in radians.
		static float atan2(float y, float x);

		// Compute a raised to power of b.
		static float pow(float a, float b);

		// Compute e raised to power.
		static float exp(float a);

		// Compute the log, base b, of v.
		static float log(float b, float v);

		// Compute natural log (base e) of a.
		static float ln(float a);

		// Compute base 10 log of a.
		static float log10(float a);

		// Compute square root of s.
		static float sqrt(float s);

		// Return the lesser value between a and b.
		static float min(float a, float b);

		// Return the lesser value between a and b.
		static int min(int a, int b);

		// Return the greater value between a and b.
		static float max(float a, float b);

		// Return the greater value between a and b.
		static int max(int a, int b);

		// Return absolute value of f.
		static float abs(float f);

		// Return absolute value of i.
		static int abs(int i);

		// Return float mod result of a % b.
		static float fmod(float a, float b);

		// Return v clamped between a and b.
		static float clamp(float a, float b, float v);

		// Return v clamped between and b.
		static int clamp(int a, int b, int v);

		// Return v clamped between 0 and 1.
		static float clamp01(float v);

		// Linear interpolation between floats.
		static float lerp(float a, float b, float amt);

		// Move float towards another float by amt.
		static float moveTowards(float a, float b, float amt);
	};
}