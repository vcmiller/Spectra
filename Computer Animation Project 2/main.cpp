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
#include "BSplineInterpolator.h"
#include "SplineSwapComponent.h"

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
	Shader *shader;
	Texture *texture;
	Material *material;
	Mesh *mesh;

	Texture *arrowTexture;
	Material *arrowMaterial;

	Texture *arrowTexture2;
	Material *arrowMaterial2;

	Mesh *arrowMesh;

	GameObject *points[40];

	void populate() {
		// Load spline
		Spline * spline = new Spline("Splines/spline1.txt");

		CatmullRomInterpolator *cinterp = new CatmullRomInterpolator(spline);
		BSplineInterpolator *binterp = new BSplineInterpolator(spline);
		RotationInterpolator *rinterp = new RotationInterpolator(spline);

		// Load assets
		shader = new Shader("Shaders/triangle");
		texture = new Texture("Textures/Ornate.jpg");
		material = new Material(shader, texture);
		mesh = new Mesh("Models/Suzanne.obj");

		arrowTexture = new Texture("Textures/PointerBlue.png");
		arrowMaterial = new Material(shader, arrowTexture);

		arrowTexture2 = new Texture("Textures/PointerGreen.png");
		arrowMaterial2 = new Material(shader, arrowTexture2);

		arrowMesh = new Mesh("Models/Pointer.obj");

		// Create display points.
		for (int i = 0; i < spline->getNumPoints(); i++) {
			// Create blue arrow at control points.
			GameObject *arrow = new GameObject();
			arrow->addComponent<MeshRenderer>()->init(arrowMesh, arrowMaterial);
			arrow->transform.setPosition(spline->getPosition(i));
			arrow->transform.setForward(cinterp->getTangent(i));
			add(arrow);

			
		}

		// Create point objects to show curve
		for (int i = 0; i < 40; i++) {
			GameObject *arrow2 = new GameObject();
			arrow2->addComponent<MeshRenderer>()->init(arrowMesh, arrowMaterial2);

			add(arrow2);

			points[i] = arrow2;

			float u = cinterp->arcToU(i / 40.0f);
			Vector3 v = cinterp->getLocation(u);

			MyQuaternion q = rinterp->getRotation(u);

			points[i]->transform.setPosition(v);
			points[i]->transform.setRotation(Quaternion(q.w, q.xyz.x, q.xyz.y, q.xyz.z));
		}

		// Create the spinning monkey object.
		GameObject *bob = new GameObject();
		bob->addComponent<MeshRenderer>()->init(mesh, material);
		bob->addComponent<SplineMovementComponent>()->init(cinterp, rinterp, &timeFunc);
		bob->addComponent<SplineSwapComponent>()->init(binterp, points, 40);

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
		add(bob);
		add(camera);
		add(light2);
	}

	// Called when the scene is unloaded.
	// Responsible for cleaning up resources and deleting objects.
	void depopulate() {
		Scene::depopulate(); // Delete objects

		// Free assets
		delete mesh;
		delete material;
		delete texture;

		delete arrowMesh;
		delete arrowMaterial;
		delete arrowMaterial2;
		delete arrowTexture;
		delete arrowTexture2;

		delete shader;
	}
};

// Main function.
void main() {
	Log::setFlush(true); // Ensures all print statements are outputted in case of crash.
	Spectra::run(new TestScene()); // Run the simulation.
}