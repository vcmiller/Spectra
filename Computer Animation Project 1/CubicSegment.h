#pragma once

#include "Vector.h"

using namespace spectra;

class CubicSegment {
public:
	CubicSegment();
	CubicSegment(float uStart, float uEnd, float start, float end, float tangentStart, float tangentEnd);

	float getPoint(float u);

private:
	float a;
	float b;
	float c;
	float d;
};