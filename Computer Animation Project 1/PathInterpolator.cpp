#include "PathInterpolator.h"

PathInterpolator::PathInterpolator(Spline * spline) {
	this->spline = spline;
}

float PathInterpolator::getTimeLimit() {
	return spline->getTime();
}
