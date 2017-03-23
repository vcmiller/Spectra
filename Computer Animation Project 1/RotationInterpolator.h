#pragma once

#include "Spline.h"
#include "MyQuaternion.h"

class RotationInterpolator {
public:
	RotationInterpolator(Spline *spline);

	MyQuaternion getRotation(float u);

private:
	List<MyQuaternion> rotations;
};