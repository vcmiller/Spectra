#pragma once
#include "Vector.h"
#include "Quaternion.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_inverse.hpp>

namespace spectra {
	class Material;
	class MeshRenderer;
	namespace internal {

		// A generic Matrix structure, with elements stored in a GLM matrix.
		// First type parameter is number of rows/cols.
		// Second is the subclass returned by matrix operations.
		// Third is the Vector subclass returned by vector operations.
		template <char S, class T, class V, class G> class Matrix {
		public:
			// Get a reference to the element at a given row and column.
			float &operator () (int row, int col) {
				return mat[col][row];
			}

			// Get the value to the element at a given row and column.
			float get(int row, int col) const {
				return mat[col][row];
			}

			// Set the value of the element at a given row and column.
			void set(int row, int col, float f) {
				mat[col][row] = f;
			}

			// Compute inverse matrix.
			T inverse() const {
				T result;
				result.mat = glm::inverse(mat);
				return result;
			}

			// Compute transpose matrix.
			T transpose() const {
				T result;
				result.mat = glm::transpose(mat);
				return result;
			}

			// Compute the inverse-transpose matrix.
			// This is often used to transform normals.
			T inverseTranspose() const {
				T result;
				result.mat = glm::inverseTranspose(mat);
				return result;
			}

			T translate(const Vector3 &vec) const {
				T result;
				result.mat = glm::translate(mat, vec.vec);
				return result;
			}

			// Multiply two matrices.
			T operator* (const T &rhs) const {
				T result;
				result.mat = mat * rhs.mat;
				return result;
			}

			// Multiply matrix by scalar.
			T operator* (float f) const {
				T result;
				result.mat = mat * f;
				return f;
			}

			// Multiply matrix by vector.
			V operator* (const V &vec) const {

				V result;
				result.vec = mat * vec.vec;
				return result;
			}

		protected:
			friend class Material;
			friend class MeshRenderer;

			// The matrix data.
			G mat;

			// Construct identity matrix.
			Matrix() {
				mat = glm::mat4(1.0f);
			}
		};

		// Output matrix to stream.
		template<char S, class T, class V, class G> std::ostream &operator<<(std::ostream &stream, const Matrix<S, T, V, G> &mat) {
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

	// A 4x4 matrix.
	class Matrix4 : public internal::Matrix<4, Matrix4, Vector4, glm::mat4> {
	public:
		// Initialize as identity.
		Matrix4();

		// Initialize all elements.
		Matrix4(float r0c0, float r0c1, float r0c2, float r0c3, 
			    float r1c0, float r1c1, float r1c2, float r1c3, 
				float r2c0, float r2c1, float r2c2, float r2c3, 
				float r3c0, float r3c1, float r3c2, float r3c3);

		// Construct a 4x4 matrix given four vectors.
		Matrix4(Vector4 c0, Vector4 c1, Vector4 c2, Vector4 c3);

		static Matrix4 transpose(Vector4 c0, Vector4 c1, Vector4 c2, Vector4 c3);

		Vector3 transformPoint(Vector3 point) const;

		Vector3 transformVector(Vector3 vector) const;

		// Construct a translation matrix.
		static Matrix4 translation(Vector3 vector);

		// Construct an euler rotation matrix.
		static Matrix4 euler(Vector3 euler);

		// Construct a scale matrix.
		static Matrix4 scale(Vector3 scale);

		// Construct rotation matrix.
		static Matrix4 rotation(Quaternion quat);

		// Construct perspective matrix.
		static Matrix4 perspective(float fovy, float aspect, float near, float far);
	};
}