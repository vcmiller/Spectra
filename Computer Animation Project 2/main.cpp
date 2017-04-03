/* 
 * Module      : Project 1
 * Author      : Vincent Miller
 * Email       : vcmiller@wpi.edu
 * Course      : CS4732
 *
 * Description : Main function for animation demo. Loads spline curve and other assets and animates model.
 *
 * Date        : 2017/23/03
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
#include "Spline.h"
#include "CatmullRomInterpolator.h"
#include "SplineMovementComponent.h"

using namespace spectra;
using namespace std;

// Component class to provide camera control.
class CameraControl : public Component {
	Vector3 euler;

	void update() override {
		// Camera movement
		float x = 0, y = 0, z = 0;

		if (Input::getKey(Key::A)) {
			x -= 1.0f;
		}

		if (Input::getKey(Key::D)) {
			x += 1.0f;
		}

		if (Input::getKey(Key::S)) {
			z -= 1.0f;
		}

		if (Input::getKey(Key::W)) {
			z += 1.0f;
		}

		if (Input::getKey(Key::Q)) {
			y -= 1.0f;
		}

		if (Input::getKey(Key::E)) {
			y += 1.0f;
		}

		transform.translate(Vector3(x, y, z) * Time::delta() * 4, Space::Local);

		// Camera rotation
		if (Input::getMouseButton(1)) {
			Vector2 md = Input::getMouseDelta();

			euler.x += -md.y;
			euler.y += md.x;
			transform.setRotation(Quaternion::euler(euler * 0.1 * FMath::toRadians));
		}
	}
};

// Maps time to distance with ease in / ease out.
float timeFunc(float f) {
	return -2.0f * f * f * f + 3.0f * f * f;
}

// Scene class, which is responsible for loading and cleaning up assets, and creating objects.
class TestScene : public Scene {
public:
	// Assets
	Shader* shader;

	Texture* goldTexture;
	Texture* floorTexture;
	
	Material* goldMat;
	Material* floorMat;

	Mesh* floor;

	Mesh* chest;
	Mesh* arm;
	Mesh* hand;
	Mesh* hammer;
	Mesh* leg1;
	Mesh* leg2;
	Mesh* foot;

	void populate() {
		// Load spline
		Spline * spline = new Spline("Splines/spline1.txt");

		CatmullRomInterpolator *cinterp = new CatmullRomInterpolator(spline);
		RotationInterpolator *rinterp = new RotationInterpolator(spline);

		// Load assets
		shader = new Shader("Shaders/triangle");
		
		goldTexture = new Texture("Textures/Gold.jpg");
		floorTexture = new Texture("Textures/Floor.jpg");

		goldMat = new Material(shader, goldTexture);
		floorMat = new Material(shader, floorTexture);

		floor = new Mesh("Models/Floor10.obj");
		chest = new Mesh("Models/Chest.obj");
		arm = new Mesh("Models/Arm.obj");
		hand = new Mesh("Models/Hand.obj");
		hammer = new Mesh("Models/Ham.obj");
		leg1 = new Mesh("Models/Leg1.obj");
		leg2 = new Mesh("Models/Leg2.obj");
		foot = new Mesh("Models/Foot.obj");

		

		// Create the spinning monkey object.
		GameObject *slamough = new GameObject();
		slamough->addComponent<MeshRenderer>()->init(chest, goldMat);
		slamough->addComponent<SplineMovementComponent>()->init(cinterp, rinterp, &timeFunc);

		// Create a camera.
		GameObject *camera = new GameObject();
		camera->addComponent<Camera>();// ->setRenderWindow(window);
		camera->transform.setPosition(Vector3(0, 1.5f, -8));
		camera->addComponent<CameraControl>();

		// Create a directional light.
		GameObject *light2 = new GameObject();
		light2->transform.setForward(Vector3(0.8f, -0.2f, 0.2f));
		Light *l2 = light2->addComponent<Light>();
		
		// Track these objects so they will be destroyed when the scene is depopulated.
		add(slamough);
		add(camera);
		add(light2);
	}

	// Called when the scene is unloaded.
	// Responsible for cleaning up resources and deleting objects.
	void depopulate() {
		Scene::depopulate(); // Delete objects

		delete chest, arm, hand, hammer, leg1, leg2, foot, floor;
		delete goldMat, floorMat;
		delete goldTexture, floorTexture;
		delete shader;
	}
};

// Main function.
void main() {
	Log::setFlush(true); // Ensures all print statements are outputted in case of crash.
	Spectra::run(new TestScene()); // Run the simulation.
}