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

using namespace spectra;
using namespace std;


// Component class that simply rotates an object on all three euler axes every frame.
class Spinner : public Component {
public:
	// Current rotation as euler angles.
	Vector3 euler;

	// Called when component is added (same as Awake() in Unity)
	void onCreate() {
		//Input::setCursorMode(CursorMode::Locked);
		euler = Vector3(0, 0, 0);
	}

	// Called every frame.
	void update() {
		float f = Time::delta() * FMath::quarterCircle; // Amount to rotate by, in radians.
		euler += Vector3(f, f, f);

		transform.setRotation(Quaternion::euler(euler));
	}
};

// Scene class, which is responsible for loading and cleaning up assets, and creating objects.
class TestScene : public Scene {
public:
	// Assets
	Shader *shader;
	Texture *texture;
	Material *material;
	Mesh *mesh;

	void populate() {

		// Load assets
		shader = new Shader("Shaders/triangle");
		texture = new Texture("Textures/Ornate.jpg");
		material = new Material(shader, texture);
		mesh = new Mesh("Models/Suzanne.obj");

		// Create the spinning monkey object.
		GameObject *bob = new GameObject();
		bob->addComponent<MeshRenderer>()->init(mesh, material);
		bob->addComponent<Spinner>();
		bob->transform.setRotation(Quaternion::euler(Vector3(0, FMath::halfCircle, 0)));
		bob->transform.setPosition(Vector3(0, 0, 0));

		// Create a camera.
		GameObject *camera = new GameObject();
		camera->addComponent<Camera>();// ->setRenderWindow(window);
		camera->transform.setPosition(Vector3(0, 0, -5));

		// Set background coor for the Camera.
		Camera *cam = camera->getComponent<Camera>();
		cam->setBackgroundColor(Color(0.2f, 0.2f, 0.2f));

		// Create a red light pointing to the right.
		GameObject *light1 = new GameObject();
		light1->transform.setForward(Vector3(1, -1, 0.9f));
		Light *l1 = light1->addComponent<Light>();
		l1->color = Color(1.0f, 0.0f, 0.0f);

		// Create a green light pointing to the left.
		GameObject *light2 = new GameObject();
		light2->transform.setForward(Vector3(-1, 1, 0.9f));
		Light *l2 = light2->addComponent<Light>();
		l2->color = Color(0.0f, 1.0f, 0.0f);
		
		// Track these objects so they will be destroyed when the scene is depopulated.
		add(bob);
		add(camera);
		add(light1);
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
		delete shader;
	}
};

// Main function.
void main() {
	Log::setFlush(true); // Ensures all print statements are outputted in case of crash.
	Spectra::run(new TestScene()); // Run the simulation.
}