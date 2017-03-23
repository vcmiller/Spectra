#include "CubicSegment.h"
#include "Matrix.h"
#include "Vector.h"
#include "Log.h"

CubicSegment::CubicSegment() {}

CubicSegment::CubicSegment(float uStart, float uEnd, float start, float end, float tangentStart, float tangentEnd) {
	Matrix4 mat(
		uStart * uStart * uStart,    uStart * uStart,    uStart,    1,
		uEnd * uEnd * uEnd,          uEnd * uEnd,        uEnd,      1,
		3 * uStart * uStart,         2 * uStart,         1,         0,
		3 * uEnd * uEnd,             2 * uEnd,           1,         0
	);

	Matrix4 m2 = mat.inverse();
	Vector4 v(start, end, tangentStart, tangentEnd);

	Vector4 result = m2 * v;
	a = result.x;
	b = result.y;
	c = result.z;
	d = result.w;

	//Log::log << "f(u) = " << a << "x^3 + " << b << "x^2 + " << c << "x + " << d << "\n";
}

float CubicSegment::getPoint(float u) {
	return 
		a * u * u * u + 
		b * u * u + 
		c * u + 
		d;
}
