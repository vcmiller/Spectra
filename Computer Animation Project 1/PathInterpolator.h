#pragma once

#include "Spline.h"
#include "Vector.h"
#include "List.h"
#include "CubicSegmentVector.h"

using namespace spectra;

class PathInterpolator {
public:
	PathInterpolator(Spline *spline);

	virtual Vector3 getLocation(float time) = 0;
	
	float getTimeLimit();

protected:
	Spline *spline;
};