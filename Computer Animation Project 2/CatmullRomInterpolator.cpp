/*
* Module      : Project 1
* Author      : Vincent Miller
* Email       : vcmiller@wpi.edu
* Course      : CS4732
*
* Description : Implementation of CatmullRomInterpolator.
*
* Date        : 2017/23/03
*
* (c) Copyright 2013, Worcester Polytechnic Institute.
*/

#include "CatmullRomInterpolator.h"
#include "FMath.h"

CatmullRomInterpolator::CatmullRomInterpolator(Spline * spline) : PathInterpolator(spline) {
	float uPer = 1.0f / (spline->getNumPoints() - 1); // Parametric value change per point in the spline.

	for (int i = 0; i < spline->getNumPoints() - 1; i++) {
		Vector3 tStart, tEnd; // Tangents.

		// Calculate first tangent.
		if (i > 0) {
			tStart = calcInternalTangent(i); // Can calculate internally.
		} else if (spline->getNumPoints() > 2) {
			tStart = (spline->getPosition(1) * 2 - spline->getPosition(2) - spline->getPosition(0)) * 1.0f; // Need to use special formula for endpoint.
		} else {
			Log::log("Error: not enough points to calculate endpoint tangents!");
		}

		// Add to stored tangents.
		tangents.add(tStart);

		// Calculate second tangent.
		if (i < spline->getNumPoints() - 2) {
			tEnd = calcInternalTangent(i + 1); // Can calculate internally.
		} else if (spline->getNumPoints() > 2) {
			// Special formula for endpoint.
			tEnd = (spline->getPosition(spline->getNumPoints() - 2) * 2 - spline->getPosition(spline->getNumPoints() - 3) - spline->getPosition(spline->getNumPoints() - 1)) * -1.0f;
			
			// Only add second tangent if it is the last point, as otherwise it will be added next loop cycle.
			tangents.add(tEnd);
		} else {
			Log::log("Error: not enough points to calculate endpoint tangents!");
		}

		// Create a set of cubic segments and add it to the list.
		segments.add(CubicSegmentVector(i * uPer, (i + 1) * uPer, spline->getPosition(i), spline->getPosition(i + 1), tStart, tEnd));
	}

	// Sample curve for forward differencing.
	sampleCurve();
}

Vector3 CatmullRomInterpolator::calcInternalTangent(int point) {
	if (point <= 0 || point > spline->getNumPoints() - 2) {
		Log::log("Error: endpoint passed to calcInternalTangent.");
	}

	// Subtract previous point from next.
	return (spline->getPosition(point + 1) - spline->getPosition(point - 1)) * 1.0f;
}

Vector3 CatmullRomInterpolator::getLocation(float u) {
	// Check for bounds.
	if (u < 0) {
		return spline->getPosition(0);
	} else if (u >= 1.0f) {
		return spline->getPosition(spline->getNumPoints() - 1);
	}

	// Parametric value per index.
	float uPer = 1.0f / (spline->getNumPoints() - 1);

	// Calculate index of correct segment.
	int index = FMath::floorToInt(u / uPer);

	// Pass calculated u to correct segment.
	return segments[index].getPoint(u);
}

Vector3 CatmullRomInterpolator::getTangent(int index) {
	return tangents[index];
}
