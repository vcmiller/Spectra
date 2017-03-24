/*
* Module      : Project 1
* Author      : Vincent Miller
* Email       : vcmiller@wpi.edu
* Course      : CS4732
*
* Description : Implementation of PathInterpolator.
*
* Date        : 2017/23/03
*
* (c) Copyright 2013, Worcester Polytechnic Institute.
*/
#include "PathInterpolator.h"
#include "FMath.h"

PathInterpolator::PathInterpolator(Spline * spline) {
	this->spline = spline;
}

float PathInterpolator::getTimeLimit() {
	return spline->getTime();
}

float PathInterpolator::arcToU(float arc) {
	// Check bounds
	if (arc <= 0) {
		return 0;
	} else if (arc >= 1) {
		return 1;
	}

	// Linear search for desired arc
	for (int i = 0; i < NUM_SAMPLES - 1; i++) {
		float t = samples[i].arc;

		if (t == arc) {
			return samples[i].u; // Special case where exact value is found.
		} else if (t < arc) {
			// If cur < arc and next > arc, found currect range.
			if (samples[i + 1].arc > arc) {
				float tNext = samples[i + 1].arc;

				// Map values.
				return FMath::lerp(samples[i].u, samples[i + 1].u, (arc - t) / (tNext - t));
			}
		}
	}

	return 1;
}

void PathInterpolator::sampleCurve() {
	float arcTotal = 0; // Total arc length so far.

	float uPer = 1.0f / (NUM_SAMPLES - 1);

	samples[0].arc = 0;
	samples[0].u = 0;

	for (int i = 1; i < NUM_SAMPLES; i++) {
		float u = i * uPer;
		Vector3 posPrev = getLocation(u - uPer);
		Vector3 pos = getLocation(u);
		float delta = (pos - posPrev).magnitude(); // Approximate arc length from previous point to current.

		arcTotal += delta;
		samples[i].arc = arcTotal;
		samples[i].u = u;
	}

	// Normalize arc lengths in range [0, 1].
	for (int i = 0; i < NUM_SAMPLES; i++) {
		samples[i].arc /= arcTotal;
	}
}
