#include "CatmullRomInterpolator.h"
#include "FMath.h"

CatmullRomInterpolator::CatmullRomInterpolator(Spline * spline) : PathInterpolator(spline) {
	float uPer = 1.0f / (spline->getNumPoints() - 1);

	for (int i = 0; i < spline->getNumPoints() - 1; i++) {
		Vector3 tStart, tEnd;

		if (i > 0) {
			tStart = calcInternalTangent(i);
		} else if (spline->getNumPoints() > 2) {
			tStart = (spline->getPosition(1) * 2 - spline->getPosition(2) - spline->getPosition(0)) * 2.0f;
		} else {
			Log::log("Error: not enough points to calculate endpoint tangents!");
		}

		tangents.add(tStart);

		if (i < spline->getNumPoints() - 2) {
			tEnd = calcInternalTangent(i + 1);
		} else if (spline->getNumPoints() > 2) {
			tEnd = (spline->getPosition(spline->getNumPoints() - 2) * 2 - spline->getPosition(spline->getNumPoints() - 3) - spline->getPosition(spline->getNumPoints() - 1)) * -2.0f;
			tangents.add(tEnd);
		} else {
			Log::log("Error: not enough points to calculate endpoint tangents!");
		}


		segments.add(CubicSegmentVector(i * uPer, (i + 1) * uPer, spline->getPosition(i), spline->getPosition(i + 1), tStart, tEnd));
	}


	sampleCurve();
}

Vector3 CatmullRomInterpolator::calcInternalTangent(int point) {
	if (point <= 0 || point > spline->getNumPoints() - 2) {
		Log::log("Error: endpoint passed to calcInternalTangent.");
	}

	return (spline->getPosition(point + 1) - spline->getPosition(point - 1)) * 2.0f;
}

Vector3 CatmullRomInterpolator::getLocation(float time) {
	if (time < 0) {
		return spline->getPosition(0);
	} else if (time >= spline->getTime()) {
		return spline->getPosition(spline->getNumPoints() - 1);
	}

	float uPer = 1.0f / (spline->getNumPoints() - 1);
	float u = time / spline->getTime();

	int index = FMath::floorToInt(u / uPer);

	return segments[index].getPoint(u);
}

Vector3 CatmullRomInterpolator::getLocationU(float u) {

	if (u < 0) {
		return spline->getPosition(0);
	} else if (u >= 1.0f) {
		return spline->getPosition(spline->getNumPoints() - 1);
	}

	float uPer = 1.0f / (spline->getNumPoints() - 1);

	int index = FMath::floorToInt(u / uPer);

	return segments[index].getPoint(u);
}

Vector3 CatmullRomInterpolator::getTangent(int index) {
	return tangents[index];
}
