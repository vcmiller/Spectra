#pragma once
#include "Vector.h"

namespace spectra {
	namespace internal {
		// A generic Matrix structure, with elements stored in a fixed array.
		// First type parameter is number of rows/cols.
		// Second is the subclass returned by matrix operations.
		// Third is the Vector subclass returned by vector operations.
		template <char S, typename T, typename V> class Matrix {
		public:
			// Get a reference to the element at a given row and column.
			float &operator () (int row, int col) {
				return data[row * S + col];
			}

			// Get the value to the element at a given row and column.
			float get(int row, int col) const {
				return data[row * S + col];
			}

			// Set the value of the element at a given row and column.
			void set(int row, int col, float f) {
				data[row * S + col] = f;
			}

			// Multiply two matrices.
			T operator* (const T &mat) const {
				T result;
				
				for (int i = 0; i < S; i++) {
					for (int j = 0; j < S; j++) {
						result(i, j) = 0;
						for (int k = 0; k < S; k++) {
							result(i, j) += get(i, k) * mat(k, j);
						}
					}
				}

				return result;
			}

			// Multiply matrix by scalar.
			T operator* (float f) const {
				T result;

				for (int i = 0; i < S * S; i++) {
					result.data[i] = data[i] * f;
				}

				return f;
			}

			// Multiply matrix by vector.
			V operator* (const V &vec) const {
				V result;

				for (int i = 0; i < S; i++) {
					for (int j = 0; j < S; j++) {
						result[i] += get(i, j) * vec.get(i);
					}
				}

				return result;
			}

			

		protected:
			// The matrix data.
			float data[S * S];

			// Construct identity matrix.
			Matrix() {
				for (int i = 0; i < S * S; i++) {
					data[i] = 0.0f;
				}

				for (int i = 0; i < S; i++) {
					set(i, i, 1.0f);
				}
			}
		};

		// Output matrix to stream.
		template<char S, typename T, typename V> std::ostream &operator<<(std::ostream &stream, const Matrix<S, T, V> &mat) {
			for (int i = 0; i < S; i++) {
				stream << "[";
				for (int j = 0; j < S; j++) {
					stream << mat.get(i, j);

					if (j < S - 1) {
						stream << ", ";
					} else {
						stream << "]\n";
					}
				}
			}

			return stream;
		}
	}

	// A 2x2 matrix.
	class Matrix2 : public internal::Matrix<2, Matrix2, Vector2> {
	public:
		// Initialize as identity.
		Matrix2();

		// Initialize all elements.
		Matrix2(float r0c0, float r0c1, float r1c0, float r1c1);
	};

	// A 3x3 matrix.
	class Matrix3 : public internal::Matrix<3, Matrix3, Vector3> {
	public:
		// Initialize as identity.
		Matrix3();

		// Initialize all elements.
		Matrix3(float r0c0, float r0c1, float r0c2, float r1c0, float r1c1, float r1c2, float r2c0, float r2c1, float r2c2);
	};

	// A 4x4 matrix.
	class Matrix4 : public internal::Matrix<4, Matrix4, Vector4> {
	public:
		// Initialize as identity.
		Matrix4();

		// Initialize all elements.
		Matrix4(float r0c0, float r0c1, float r0c2, float r0c3, float r1c0, float r1c1, float r1c2, float r1c3, float r2c0, float r2c1, float r2c2, float r2c3, float r3c0, float r3c1, float r3c2, float r3c3);
	};
}