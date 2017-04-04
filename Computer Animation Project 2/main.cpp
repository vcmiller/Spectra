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
#include "BoneRotation.h"

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

		transform.translate(Vector3(x, y, z) * Time::delta() * 10, Space::Local);

		// Camera rotation
		if (Input::getMouseButton(1)) {
			Vector2 md = Input::getMouseDelta();

			euler.x += -md.y;
			euler.y += md.x;
			transform.setRotation(Quaternion::euler(euler * 0.1 * FMath::toRadians));
		}
	}
};

GameObject* createChild(GameObject* parent, Mesh* mesh, Material* material, Vector3 localPos) {
	GameObject *obj = new GameObject();
	obj->addComponent<MeshRenderer>()->init(mesh, material);
	obj->transform.setParent(&parent->transform);
	obj->transform.setLocalPosition(localPos);

	return obj;
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

	Mesh* floorMesh;

	Mesh* chestMesh;
	Mesh* armMesh;
	Mesh* handMesh;
	Mesh* hammerMesh;
	Mesh* leg1Mesh;
	Mesh* leg2Mesh;
	Mesh* footMesh;

	void populate() {
		// Load spline
		Spline * spline = new Spline("Splines/spline2.txt");

		CatmullRomInterpolator *cinterp = new CatmullRomInterpolator(spline);
		RotationInterpolator *rinterp = new RotationInterpolator(spline);

		// Load assets
		shader = new Shader("Shaders/triangle");
		
		goldTexture = new Texture("Textures/Gold.jpg");
		floorTexture = new Texture("Textures/Floor.jpg");

		goldMat = new Material(shader, goldTexture);
		floorMat = new Material(shader, floorTexture);

		floorMesh = new Mesh("Models/Floor10.obj");
		chestMesh = new Mesh("Models/Chest.obj");
		armMesh = new Mesh("Models/Arm.obj");
		handMesh = new Mesh("Models/Hand.obj");
		hammerMesh = new Mesh("Models/Ham.obj");
		leg1Mesh = new Mesh("Models/Leg1.obj");
		leg2Mesh = new Mesh("Models/Leg2.obj");
		footMesh = new Mesh("Models/Foot.obj");

		GameObject* floor = new GameObject();
		floor->addComponent<MeshRenderer>()->init(floorMesh, floorMat);
		floor->transform.setPosition(Vector3(0, -6, 0));
		

		// Create the spinning monkey object.
		GameObject *slamough = new GameObject();
		slamough->addComponent<MeshRenderer>()->init(chestMesh, goldMat);
		slamough->transform.setRotation(Quaternion::euler(Vector3(0, FMath::halfCircle, 0)));
		slamough->addComponent<SplineMovementComponent>()->init(cinterp, rinterp, nullptr);

		GameObject* rightLeg1 = createChild(slamough, leg1Mesh, goldMat, Vector3(0.9f, -2.5f, 0.2f));
		GameObject* rightLeg2 = createChild(rightLeg1, leg2Mesh, goldMat, Vector3(0.3f, -1.3f, 0.2f));
		GameObject* rightFoot = createChild(rightLeg2, footMesh, goldMat, Vector3(0.0f, -1.3f, -0.4f));

		GameObject* leftLeg1 = createChild(slamough, leg1Mesh, goldMat, Vector3(-0.9f, -2.5f, 0.2f));
		GameObject* leftLeg2 = createChild(leftLeg1, leg2Mesh, goldMat, Vector3(0.3f, -1.3f, 0.2f));
		GameObject* leftFoot = createChild(leftLeg2, footMesh, goldMat, Vector3(0.0f, -1.3f, -0.4f));
		leftLeg1->transform.setLocalScale(Vector3(-1, 1, 1));

		GameObject* rightArm = createChild(slamough, armMesh, goldMat, Vector3(1.5f, -0.1f, -0.2f));
		GameObject* rightHand = createChild(rightArm, handMesh, goldMat, Vector3(1.0f, -1.0f, 0.0f));

		GameObject* leftArm = createChild(slamough, armMesh, goldMat, Vector3(-1.5f, -0.1f, -0.2f));
		GameObject* leftHand = createChild(leftArm, handMesh, goldMat, Vector3(1.0f, -1.0f, 0.0f));
		GameObject* hammer = createChild(leftHand, hammerMesh, goldMat, Vector3(-1.0f, 0.0f, 1.5f));
		leftArm->transform.setLocalScale(Vector3(-1, 1, 1));


		rightLeg1->addComponent<BoneRotation>()->init([](float time) {
			return Quaternion::euler(Vector3(FMath::sin(time * 4) / 2, 0, 0));
		});

		rightLeg2->addComponent<BoneRotation>()->init([](float time) {
			return Quaternion::euler(Vector3(FMath::cos(time * 4) / -1.5f, 0, 0));
		});

		rightFoot->addComponent<BoneRotation>()->init([](float time) {
			return Quaternion::euler(Vector3(FMath::sin(time * 4 - 2) / -2, 0, 0));
		});

		leftLeg1->addComponent<BoneRotation>()->init([](float time) {
			return Quaternion::euler(Vector3(-FMath::sin(time * 4) / 2, 0, 0));
		});

		leftLeg2->addComponent<BoneRotation>()->init([](float time) {
			return Quaternion::euler(Vector3(-FMath::cos(time * 4) / -1.5f, 0, 0));
		});

		leftFoot->addComponent<BoneRotation>()->init([](float time) {
			return Quaternion::euler(Vector3(-FMath::sin(time * 4 - 2) / -2, 0, 0));
		});

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

		delete chestMesh, armMesh, handMesh, hammerMesh, leg1Mesh, leg2Mesh, footMesh, floorMesh;
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