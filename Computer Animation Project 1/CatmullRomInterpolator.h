#pragma once

#include "PathInterpolator.h"

class CatmullRomInterpolator : public PathInterpolator {
public:
	CatmullRomInterpolator(Spline *spline);


	virtual Vector3 getLocation(float time) override;
	Vector3 getLocationU(float u);

	Vector3 getTangent(int index);

private:

	Vector3 calcInternalTangent(int point);

	List <Vector3> tangents;
	List<CubicSegmentVector> segments;
};