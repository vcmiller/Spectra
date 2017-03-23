#include "PathInterpolator.h"
#include "FMath.h"

PathInterpolator::PathInterpolator(Spline * spline) {
	this->spline = spline;
}

float PathInterpolator::getTimeLimit() {
	return spline->getTime();
}

float PathInterpolator::arcToU(float arc) {
	int start = 0;
	int end = NUM_SAMPLES;

	if (arc <= 0) {
		return 0;
	} else if (arc >= 1) {
		return 1;
	}

	for (int i = 0; i < NUM_SAMPLES - 1; i++) {
		float t = samples[i].arc;

		if (t == arc) {
			return samples[i].u;
		} else if (t < arc) {
			if (samples[i + 1].arc > arc) {
				float tNext = samples[i + 1].arc;

				return FMath::lerp(samples[i].u, samples[i + 1].u, (arc - t) / (tNext - t));
			}
		}
	}

	return 1;
}

void PathInterpolator::sampleCurve() {
	float arcTotal = 0;

	float uPer = 1.0f / (NUM_SAMPLES - 1);

	samples[0].arc = 0;
	samples[0].u = 0;

	for (int i = 1; i < NUM_SAMPLES; i++) {
		float u = i * uPer;
		Vector3 posPrev = getLocationU(u - uPer);
		Vector3 pos = getLocationU(u);
		float delta = (pos - posPrev).magnitude();

		arcTotal += delta;
		samples[i].arc = arcTotal;
		samples[i].u = u;
	}

	for (int i = 0; i < NUM_SAMPLES; i++) {
		samples[i].arc /= arcTotal;
	}
}
