#include "Flocking.h"
#include "Time.h"

void Flocking::init(float range, List<Flocking*>* others, int species) {
	this->range = range;
	this->others = others;
	this->species = species;
}

void Flocking::addForce(Vector3(*function)(Flocking *self, Flocking *other)) {
	interactiveForces.add(function);
}

void Flocking::addSelfForce(Vector3(*function)(Flocking *self)) {
	selfForces.add(function);
}

void Flocking::update() {
	for (auto func : interactiveForces) {
		Vector3 totalForce = Vector3::zero;
		float c = 0;

		for (auto other : *others) {

			if (other != this) {
				if (Vector3::sqrDistance(transform.getPosition(), other->transform.getPosition()) < range * range) {
					totalForce += func(this, other);

					c++;
				}
			}
		}

		if (c > 0) {
			totalForce /= c;
		}

		velocity += totalForce * Time::delta();
	}

	for (auto func : selfForces) {
		velocity += func(this) * Time::delta();
	}
	
	//transform.setForward(velocity.normalized());
	transform.translate(velocity * Time::delta());

	
}
