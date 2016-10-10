#include "Property.h"
#include "Vector.h"
#include "Matrix.h"
#include "Log.h"
#include "List.h"

using namespace util;
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
	Vector3 vec(1, 0, 0);

	Vector3 vec2(0, 1, 0);

	Vector3 r = vec + vec2;
	float halfpi = 3.141592f / 2.0f;

	Quaternion q1 = Quaternion::euler(Vector3(0, halfpi, 0));
	Quaternion q2 = Quaternion::euler(Vector3(0, 0, 0));

	Log::log(Quaternion::angle(q1, q2));
	glm::vec3 v1(2, 3, 0);
	glm::vec3 v2(3, 1, 2);

	List<int> list;
	list.add(2);
	list.add(3);
	list.add(4);

	for (int i : list) {
		Log::log << i << "\n";
	}
}