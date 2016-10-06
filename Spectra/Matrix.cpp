#include "Matrix.h"

namespace spectra {
	Matrix2::Matrix2() { }

	Matrix2::Matrix2(float r0c0, float r0c1, float r1c0, float r1c1) {
		set(0, 0, r0c0);
		set(0, 1, r0c1);

		set(1, 0, r1c0);
		set(1, 1, r1c1);
	}

	Matrix3::Matrix3() { }

	Matrix3::Matrix3(float r0c0, float r0c1, float r0c2, float r1c0, float r1c1, float r1c2, float r2c0, float r2c1, float r2c2) {
		set(0, 0, r0c0);
		set(0, 1, r0c1);
		set(0, 2, r0c2);

		set(1, 0, r1c0);
		set(1, 1, r1c1);
		set(1, 2, r1c2);

		set(2, 0, r2c0);
		set(2, 1, r2c1);
		set(2, 2, r2c2);
	}

	Matrix4::Matrix4() { }

	Matrix4::Matrix4(float r0c0, float r0c1, float r0c2, float r0c3, float r1c0, float r1c1, float r1c2, float r1c3, float r2c0, float r2c1, float r2c2, float r2c3, float r3c0, float r3c1, float r3c2, float r3c3) {
		set(0, 0, r0c0);
		set(0, 1, r0c1);
		set(0, 2, r0c2);
		set(0, 3, r0c3);

		set(1, 0, r1c0);
		set(1, 1, r1c1);
		set(1, 2, r1c2);
		set(1, 3, r1c3);

		set(2, 0, r2c0);
		set(2, 1, r2c1);
		set(2, 2, r2c2);
		set(2, 3, r2c3);

		set(3, 0, r3c0);
		set(3, 1, r3c1);
		set(3, 2, r3c2);
		set(3, 3, r3c3);
	}

}