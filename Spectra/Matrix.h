#pragma once
#include "Vector.h"

namespace spectra {
	namespace internal {
		template <char S, typename T, typename V> class Matrix {
		public:
			float &operator () (int row, int col) {
				return data[row * S + col];
			}

			float get(int row, int col) const {
				return data[row * S + col];
			}

			void set(int row, int col, float f) {
				data[row * S + col] = f;
			}

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

			T operator* (float f) const {
				T result;

				for (int i = 0; i < S * S; i++) {
					result.data[i] = data[i] * f;
				}

				return f;
			}

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
			float data[S * S];

			Matrix() {
				for (int i = 0; i < S * S; i++) {
					data[i] = 0.0f;
				}

				for (int i = 0; i < S; i++) {
					set(i, i, 1.0f);
				}
			}
		};

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

	class Matrix2 : public internal::Matrix<2, Matrix2, Vector2> {
	public:
		Matrix2();

		Matrix2(float r0c0, float r0c1, float r1c0, float r1c1);
	};

	class Matrix3 : public internal::Matrix<3, Matrix3, Vector3> {
	public:
		Matrix3();

		Matrix3(float r0c0, float r0c1, float r0c2, float r1c0, float r1c1, float r1c2, float r2c0, float r2c1, float r2c2);
	};

	class Matrix4 : public internal::Matrix<4, Matrix4, Vector4> {
	public:
		Matrix4();

		Matrix4(float r0c0, float r0c1, float r0c2, float r0c3, float r1c0, float r1c1, float r1c2, float r1c3, float r2c0, float r2c1, float r2c2, float r2c3, float r3c0, float r3c1, float r3c2, float r3c3);
	};
}