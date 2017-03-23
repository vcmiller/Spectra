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
#include "Spline.h"
#include "CatmullRomInterpolator.h"

using namespace spectra;
using namespace std;


class CameraControl : public Component {
	void update() override {
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

		if (Input::getMouseButton(1)) {
			Vector2 md = Input::getMouseDelta();
			transform.rotate(Vector3(-md.y, md.x, 0) * 0.1f * FMath::toRadians, Space::Local);
		}
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

	Texture *arrowTexture;
	Material *arrowMaterial;

	Texture *arrowTexture2;
	Material *arrowMaterial2;

	Mesh *arrowMesh;

	void populate() {
		Spline * spline = new Spline("Splines/spline2.txt");

		CatmullRomInterpolator interp(spline);

		// Load assets
		shader = new Shader("Shaders/triangle");
		texture = new Texture("Textures/Ornate.jpg");
		material = new Material(shader, texture);
		mesh = new Mesh("Models/Quad.obj");

		arrowTexture = new Texture("Textures/PointerBlue.png");
		arrowMaterial = new Material(shader, arrowTexture);

		arrowTexture2 = new Texture("Textures/PointerGreen.png");
		arrowMaterial2 = new Material(shader, arrowTexture2);

		arrowMesh = new Mesh("Models/Pointer.obj");
		//CatmullRomInterpolator interp(spline);

		for (int i = 0; i < spline->getNumPoints(); i++) {
			GameObject *arrow = new GameObject();
			arrow->addComponent<MeshRenderer>()->init(arrowMesh, arrowMaterial);
			arrow->transform.setPosition(spline->getPosition(i));
			arrow->transform.setForward(interp.getTangent(i));
			add(arrow);

			if (i < spline->getNumPoints() - 1) {
				for (int j = 0; j < 8; j++) {
					GameObject *arrow2 = new GameObject();
					arrow2->addComponent<MeshRenderer>()->init(arrowMesh, arrowMaterial2);

					float u = (1.0f / (spline->getNumPoints() - 1)) * (i + 0.125f * j);

					Vector3 v = interp.getLocationU(u);

					arrow2->transform.setPosition(v);
					//arrow2->transform.setForward(Vector3::forward);
					add(arrow2);
				}
			}
			
		}


		

		// Create the spinning monkey object.
		//GameObject *bob = new GameObject();
		//bob->addComponent<MeshRenderer>()->init(mesh, material);
		//bob->addComponent<Spinner>();
		//bob->transform.setRotation(Quaternion::euler(Vector3(0, FMath::halfCircle, 0)));
		//bob->transform.setPosition(Vector3(0, 0, 0));

		// Create a camera.
		GameObject *camera = new GameObject();
		camera->addComponent<Camera>();// ->setRenderWindow(window);
		camera->transform.setPosition(Vector3(0, 1.5f, -8));
		camera->addComponent<CameraControl>();

		// Create a green light pointing to the left.
		GameObject *light2 = new GameObject();
		light2->transform.setForward(Vector3(0.8f, -0.2f, 0.2f));
		Light *l2 = light2->addComponent<Light>();
		
		// Track these objects so they will be destroyed when the scene is depopulated.
		//add(bob);
		add(camera);
		//add(light1);
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