#include "RotationInterpolator.h"
#include "FMath.h"

using namespace spectra;

RotationInterpolator::RotationInterpolator(Spline * spline) {
	for (int i = 0; i < spline->getNumPoints(); i++) {
		rotations.add(MyQuaternion::euler(spline->getRotation(i)));
	}
}

MyQuaternion RotationInterpolator::getRotation(float u) {
	if (u <= 0) {
		return rotations[0];
	} else {
		return rotations[rotations.length() - 1];
	}

	float uPer = 1.0f / (rotations.length() - 1);
	int i = FMath::floorToInt(u / uPer);
	int i2 = i + 1;

	float uMin = i * uPer;
	float uMax = i2 * uPer;

	float a = (u - uMin) / (uMax - uMin);
	return MyQuaternion::slerp(rotations[i], rotations[i2], a);
}
