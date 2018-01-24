#pragma once

#include "Component.h"
#include "Vector.h"

using namespace spectra;

class Flocking : public Component {
public:
	void init(float range, List<Flocking*>* others, int species);

	Vector3 velocity;
	float range;
	List<Flocking*>* others;
	int species;

	void addForce(Vector3(*function)(Flocking* self, Flocking* other));
	void addSelfForce(Vector3(*function)(Flocking* self));

	void update() override;

	static const int RED = 0;
	static const int GREEN = 1;
	static const int BLUE = 2;

private:
	
	List<Vector3(*)(Flocking* self, Flocking* other)> interactiveForces;
	List<Vector3(*)(Flocking* self)> selfForces;
};