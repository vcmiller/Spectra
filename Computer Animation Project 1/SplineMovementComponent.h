#pragma once

#include "Component.h"
#include "PathInterpolator.h"
#include "RotationInterpolator.h"

using namespace spectra;

class SplineMovementComponent : public Component {
public:
	void init(PathInterpolator *path, RotationInterpolator *rotation, bool play = true, bool loop = true);

	virtual void update() override;

	bool playing;
	bool loop;
	float timePosition;

	PathInterpolator *path;
	RotationInterpolator *rotation;
};