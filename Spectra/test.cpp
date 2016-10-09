#include "Property.h"
#include "Vector.h"
#include "Matrix.h"

#include <iostream>

using namespace util;
using namespace spectra::internal;
using namespace spectra;
using namespace std;

class Test {
public:
	Property<int> prop{ &val, getter, setter };

	Test() {
	}

private:
	int val;

	static int getter(int val) {
		cout << "Getter!\n";
		return val;
	}

	static int setter(int val) {
		cout << "Setter!\n";
		return val;
	}
};

void main() {
	Vector3 vec(2, 3, 0);

	Vector3 vec2(3, 1, 2);

	Vector3 r = vec + vec2;

	std::cout << Vector3::lerp(vec, vec2, 0.5f) << std::endl;
	glm::vec3 v1(2, 3, 0);
	glm::vec3 v2(3, 1, 2);
}