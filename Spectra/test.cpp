#include "Property.h"
#include "Vector.h"
#include "Matrix.h"

#include <iostream>

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
	Vector3 vec(2, 3, 0);

	cout << vec << std::endl << std::endl;

	Matrix3 mat;

	cout << mat << std::endl;

	cout << mat * vec << std::endl;
}