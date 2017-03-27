#include "SplineSwapComponent.h"
#include "Input.h"
#include "Key.h"

void SplineSwapComponent::onCreate() {
	spline = getComponent<SplineMovementComponent>();
}

void SplineSwapComponent::update() {
	// If user presses space bar, swap splines and store old one in alt.
	if (Input::getKeyDown(Key::Space)) {
		PathInterpolator *t = spline->path;
		spline->path = alt;
		alt = t;
	}
}

void SplineSwapComponent::init(PathInterpolator * alt) {
	this->alt = alt;
}
