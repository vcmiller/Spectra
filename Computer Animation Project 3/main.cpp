/* 
 * Module      : Project 0
 * Author      : Vincent Miller
 * Email       : vcmiller@wpi.edu
 * Course      : CS4732
 *
 * Description : Main function for animation demo. Loads an object from an OBJ file and displays it rotating on the screen.
 *
 * Date        : 2017/17/03
 *
 * (c) Copyright 2013, Worcester Polytechnic Institute.
 */

/* -- INCLUDE FILES ------------------------------------------------------ */

#include "Property.h"
#include "Vector.h"
#include "Matrix.h"
#include "Log.h"
#include "List.h"
#include "Object.h"
#include "ObjectSet.h"
#include "Config.h"
#include "Spectra.h"
#include "Clock.h"
#include "Time.h"
#include "Scene.h"
#include "GameObject.h"
#include "Component.h"
#include "Window.h"
#include "Material.h"
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Input.h"
#include "Key.h"
#include "FMath.h"
#include "Light.h"
#include "Flocking.h"

using namespace spectra;
using namespace std;

const float cohesionStrength = 3.0f;
const float alignmentStrength = .6f;
const float separationStrength = 2.4f;
const float wallAvoidStrength = 10.0f;
const float speedCorrectionStrength = 1;
const float desiredSpeed = 2;
const float predatorStrength = 2.0f;
const float preyStrength = 2.0f;

const float bound = 5.0f;

const float range = 2.0f;

Vector3 predator(Flocking* self, Flocking* other) {
	if (other->species == (self->species + 2) % 3) {
		return (other->transform.getPosition() - self->transform.getPosition()).normalized() * predatorStrength;
	} else {
		return Vector3::zero;
	}
}

Vector3 prey(Flocking* self, Flocking* other) {
	if (other->species == (self->species + 1) % 3) {
		return (self->transform.getPosition() - other->transform.getPosition()).normalized() * preyStrength;
	} else {
		return Vector3::zero;
	}
}

Vector3 speedCorrection(Flocking* self) {
	Vector3 v = self->velocity.normalized() * desiredSpeed;
	return (v - self->velocity) * speedCorrectionStrength;
}

Vector3 wallAvoid(Flocking* self) {
	Vector3 v = self->transform.getPosition();
	Vector3 f = Vector3::zero;

	if (v.x < -bound) {
		f.x += -bound - v.x;
	}

	if (v.x > bound) {
		f.x += bound - v.x;
	}

	if (v.y < -bound) {
		f.y += -bound - v.y;
	}

	if (v.y > bound) {
		f.y += bound - v.y;
	}

	return f * wallAvoidStrength;
}

Vector3 cohesion(Flocking* self, Flocking* other) {

	if (other->species == self->species) {
		return (other->transform.getPosition() - self->transform.getPosition()).normalized() * cohesionStrength;
	} else {
		return Vector3::zero;
	}
}

Vector3 alignment(Flocking* self, Flocking* other) {
	if (other->species == self->species) {
		return (other->velocity - self->velocity).normalized() * alignmentStrength;
	} else {
		return Vector3::zero;
	}
	
}

Vector3 separation(Flocking* self, Flocking* other) {
	if (other->species == self->species) {
		Vector3 v = self->transform.getPosition() - other->transform.getPosition();
		float f = v.sqrMagnitude();
		if (f < 0.1f) {
			f = 0.1f;
		}
		return (v / v.sqrMagnitude()) * separationStrength;
	} else {
		return Vector3::zero;
	}
	
}


// Scene class, which is responsible for loading and cleaning up assets, and creating objects.
class TestScene : public Scene {
public:
	// Assets
	Shader *shader;
	Texture *textureRed;
	Texture *textureGreen;
	Texture *textureBlue;
	Material *materialRed;
	Material *materialGreen;
	Material *materialBlue;
	Mesh *mesh;

	List<Flocking*> birds;

	Flocking* make(Vector3 pos, Vector3 vel, int species) {
		GameObject *b = new GameObject();
		b->transform.setPosition(Vector3(1, 1, 0));

		if (species == 0) {
			b->addComponent<MeshRenderer>()->init(mesh, materialRed);
		} else if (species == 1) {
			b->addComponent<MeshRenderer>()->init(mesh, materialGreen);
		} else if (species == 2) {
			b->addComponent<MeshRenderer>()->init(mesh, materialBlue);
		}
		Flocking *f = b->addComponent<Flocking>();
		birds.add(f);
		f->init(range, &birds, species);
		f->velocity = vel;
		f->transform.setPosition(pos);
		add(b);

		f->addForce(cohesion);
		f->addForce(separation);
		f->addForce(alignment);
		f->addForce(predator);
		f->addForce(prey);
		f->addSelfForce(wallAvoid);
		f->addSelfForce(speedCorrection);

		f->transform.setForward(Vector3::back);

		return f;
	}

	void populate() {
		shader = new Shader("Shaders/triangle");
		mesh = new Mesh("Models/Pointer.obj");
		textureRed = new Texture("Textures/PointerRed.png");
		textureGreen = new Texture("Textures/PointerGreen.png");
		textureBlue = new Texture("Textures/PointerBlue.png");
		materialRed = new Material(shader, textureRed);
		materialGreen = new Material(shader, textureGreen);
		materialBlue = new Material(shader, textureBlue);

		// Create a camera.
		GameObject *camera = new GameObject();
		camera->addComponent<Camera>();// ->setRenderWindow(window);
		camera->transform.setPosition(Vector3(0, 0, -15));

		// Set background coor for the Camera.
		Camera *cam = camera->getComponent<Camera>();
		cam->setBackgroundColor(Color(1, 1, 1));

		// Create a green light pointing to the left.
		GameObject *light2 = new GameObject();
		light2->transform.setPosition(Vector3(0, 1, 0));
		Light *l2 = light2->addComponent<Light>();

		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				make(Vector3(i - 2.5f, j - 2.5f, 0) * 1.3f, Vector3((i + j) % 2 - 1, -1, 0), 0);
			}
		}

		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				make(Vector3(i - 2.5f, j - 2.5f, 0) * 1.0f, Vector3((i + j) % 2 - 1, -1, 0), 1);
			}
		}

		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 4; j++) {
				make(Vector3(i - 2.5f, j - 2.5f, 0) * 1.6f, Vector3((i + j) % 2 - 1, -1, 0), 2);
			}
		}
		
		// Track these objects so they will be destroyed when the scene is depopulated.
		add(camera);
		add(light2);
	}

	// Called when the scene is unloaded.
	// Responsible for cleaning up resources and deleting objects.
	void depopulate() {
		Scene::depopulate(); // Delete objects

		delete mesh;
		delete materialRed, materialGreen, materialBlue;
		delete textureRed, textureGreen, textureBlue;
		delete shader;
	}
};

// Main function.
void main() {
	Log::setFlush(true); // Ensures all print statements are outputted in case of crash.
	Spectra::run(new TestScene()); // Run the simulation.
}