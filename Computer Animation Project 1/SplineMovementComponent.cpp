#include "SplineMovementComponent.h"
#include "Time.h"
#include "FMath.h"

void SplineMovementComponent::init(PathInterpolator * path, RotationInterpolator * rotation, bool play, bool loop) {
	this->path = path;
	this->rotation = rotation;
	this->playing = play;
	this->loop = loop;
	this->timePosition = 0.0f;
}

void SplineMovementComponent::update() {
	if (playing) {
		timePosition += Time::delta();
	}

	if (loop) {
		timePosition = FMath::fmod(timePosition, path->getTimeLimit());
	}

	float tNorm = timePosition / path->getTimeLimit();


}
