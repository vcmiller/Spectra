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

		for (int i = 0; i < numPoints; i++) {
			float u = spline->path->arcToU(i / 40.0f);
			Vector3 v = spline->path->getLocation(u);

			MyQuaternion q = spline->rotation->getRotation(u);

			points[i]->transform.setPosition(v);
			points[i]->transform.setRotation(Quaternion(q.w, q.xyz.x, q.xyz.y, q.xyz.z));
		}
	}
}

void SplineSwapComponent::init(PathInterpolator * alt, GameObject **points, int numPoints) {
	this->alt = alt;
	this->points = points;
	this->numPoints = numPoints;
}
