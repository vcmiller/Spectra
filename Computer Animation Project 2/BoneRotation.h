#pragma once

#include "Component.h"
#include "Vector.h"

using namespace spectra;

class BoneRotation : public Component {
public:
	void update() override;

	void init(Quaternion(*function)(float t));

	Quaternion(*function)(float t);
};