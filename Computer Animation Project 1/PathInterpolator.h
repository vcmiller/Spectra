#pragma once

#include "Spline.h"
#include "Vector.h"
#include "List.h"
#include "CubicSegmentVector.h"

#define NUM_SAMPLES 256

using namespace spectra;

class PathInterpolator {
public:
	PathInterpolator(Spline *spline);

	virtual Vector3 getLocation(float u) = 0;
	
	float getTimeLimit();

	float arcToU(float arc);

protected:
	Spline *spline;

	void sampleCurve();

	struct Sample {
		float u;
		float arc;
	};

	Sample samples[NUM_SAMPLES];
};