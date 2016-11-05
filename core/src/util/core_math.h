#pragma once

#include<initializer_list>
#include<cmath>
#include<global_config.h>

namespace core {

	namespace literal {
		constexpr f32 pi = 3.14159265f;
		constexpr f32 half_pi = pi * 0.5f;
		constexpr f32 pi_one_and_half = pi * 1.5f;
		constexpr f32 pi2 = pi * 2.0f;
	}

	template<class T>
	struct APICALL vector3 {
		union {
			T array[3];
			struct {
				T x;
				T y; 
				T z;
			};

			struct {
				T r;
				T g;
				T b;
			};
		};

		vector3() = default;

		vector3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {} 

		vector3(std::initializer_list<T> l) {
			auto it = l.begin();
			for(int i = 0; it != l.end() && i < 3; ++i, ++it) {
				array[i] = *it;
			}
		}

		vector3<T> & operator=(const vector3<T> & v) {
			array[0] = v.array[0];
			array[1] = v.array[1];
			array[2] = v.array[2];
			return *this;
		}
		vector3<T> & operator+=(const vector3<T> & v) {
			array[0] += v.array[0];
			array[1] += v.array[1];
			array[2] += v.array[2];
			return *this;
		}
		vector3<T> & operator-=(const vector3<T> & v) {
			array[0] -= v.array[0];
			array[1] -= v.array[1];
			array[2] -= v.array[2];
			return *this;
		}


		vector3<T> & operator=(T t) {
			x = t;
			y = t;
			z = t;
			return *this;
		}

		vector3<T> & operator+=(T t) {
			x += t;
			y += t;
			z += t;
			return *this;
		}
		vector3<T> & operator-=(T t) {
			x -= t;
			y -= t;
			z -= t;
			return *this;
		}
		vector3<T> & operator*=(T t) {
			x *= t;
			y *= t;
			z *= t;
			return *this;
		}
		vector3<T> & operator/=(T t) {
			x /= t;
			y /= t;
			z /= t;
			return *this;
		}

		T & operator[](u32 i) {
			return array[i];
		}
		const T & operator[](u32 i) const {
			return array[i];
		}

		T length() const {
			return sqrtf(x*x + y*y + z*z);
		}

		T length_with_no_sqrt() const {
			return x*x + y*y + z*z;
		}

		void normalize() {
			auto l = length();
			l = 1.0f / l;
			x *= l;
			y *= l;
			z *= l;
		}
	};

	template<class T>
	vector3<T>  APICALL inline operator+(const vector3<T> & v, const vector3<T> & r) {
		vector3<T> ret;
		ret[0] = v.x + r.x;
		ret[1] = v.y + r.y;
		ret[2] = v.z + r.z;
		return ret;
	}
	template<class T>
	vector3<T>  APICALL inline operator-(const vector3<T> & v, const vector3<T> & r) {
		vector3<T> ret;
		ret[0] = v.x - r.x;
		ret[1] = v.y - r.y;
		ret[2] = v.z - r.z;
		return ret;
	}

	// dot product
	template<class T>
	T  APICALL inline operator*(const vector3<T> & v, const vector3<T> & r) {
		return v.x * r.x + v.y * r.y + v.z * r.z;
	}

	// cross product
	template<class T>
	vector3<T>  APICALL inline operator^(const vector3<T> & v, const vector3<T> & r) {
		vector3<T> ret;
		ret[0] = v[1] * r[2] - v[2] * r[1];
		ret[1] = v[2] * r[0] - v[0] * r[2];
		ret[2] = v[0] * r[1] - v[1] * r[0];
		return ret;
	}

	template<class T>
	struct APICALL vector4 {
		union {
			T array[4];
			struct {
				T x;
				T y;
				T z;
				T w;
			};
			struct {
				T r;
				T g;
				T b;
				T a;
			};
		};

		T & operator[](u32 i) {
			return array[i];
		}
		const T & operator[](u32 i) const {
			return array[i];
		}

		vector4<T> & operator=(const vector4<T> & v) {
			x = v.x;
			y = v.y;
			z = v.z;
			w = v.w;
			return *this;
		}
		vector4<T> & operator+=(const vector4<T> & v) {
			x += v.x;
			y += v.y;
			z += v.z;
			w += v.w;
			return *this;
		}
		vector4<T> & operator-=(const vector4<T> & v) {
			x -= v.x;
			y -= v.y;
			z -= v.z;
			w -= v.w;
			return *this;
		}

		vector4<T> & operator=(T t) {
			x = t;
			y = t;
			z = t;
			w = t;
			return *this;
		}

		vector4<T> & operator+=(T t) {
			x += t;
			y += t;
			z += t;
			w += t;
			return *this;
		}
		vector4<T> & operator-=(T t) {
			x -= t;
			y -= t;
			z -= t;
			w -= t;
			return *this;
		}
		vector4<T> & operator*=(T t) {
			x *= t;
			y *= t;
			z *= t;
			w *= t;
			return *this;
		}
		vector4<T> & operator/=(T t) {
			x /= t;
			y /= t;
			z /= t;
			w /= t;
			return *this;
		}
	};

	template<class T>
	struct APICALL matrix4x4 {
		union {
			T array1D[16];
			vector4<T> array2D[4];
		};

		matrix4x4() = default;

		matrix4x4(std::initializer_list<T> l) {
			auto it = l.begin();
			for(int i = 0; it != l.end() && i < 16; ++i, ++it) {
				array1D[i] = *it;
			}
		}

		void zero() {
			for(int i = 0; i < 16; ++i) array1D[i] = 0;
		}

		void identity() {
			for(int i = 0; i < 4; ++i) {
				for(int j = 0; j < 4; ++j) {
					if(i == j) {
						array2D[i][j] = 1;
					}
					else {
						array2D[i][j] = 0;
					}
				}
			}
		}

		void transpose() {
			for(int i = 0; i < 4; ++i) {
				for(int j = 0; j < 4; ++j) {
					if(i != j) {
						T tmp = array2D[i][j];
						array2D[i][j] = array2D[j][i];
						array2D[j][i] = tmp;
					}
				}
			} 
		}

		// vec must be normalized
		void rot(const vector3<T> & n, T theta) {
			array1D[3] = array1D[7] = array1D[11] = array1D[12] = array1D[13] = array1D[14] = 0;
			array1D[15] = 1;
			auto cs = 1.0f - std::cosf(theta);
			auto c = std::cosf(theta);
			auto s = std::sinf(theta);
			auto xy = n.x * n.y;
			auto xz = n.x * n.z;
			auto zs = n.z * s;
			auto ys = n.y * s;
			auto xs = n.x * s; 
			array1D[0] = ((n.x * n.x) * cs) + c;
			array1D[1] = (xy * cs) - zs;
			array1D[2] = (xz * cs) + ys; 
			array1D[4] = (xy * cs) + zs;
			array1D[5] = ((n.y * n.y) * cs) + c;
			array1D[6] = (xz * cs) - xs; 
			array1D[8] = (xz * cs) - ys;
			array1D[9] = ((n.y * n.z) * cs) + xs;
			array1D[10] = ((n.z * n.z) * cs) + c;			
		}

		void scale(const vector3<T> & vec) {
			zero();
			array2D[0][0] = vec[0];
			array2D[1][1] = vec[1];
			array2D[2][2] = vec[2];
			array2D[3][3] = 1;
		}

		vector4<T> & operator[](u32 i) {
			return array2D[i];
		}

		T & operator()(u32 i) {
			return array1D[i];
		}

		const T & operator()(u32 i) const {
			return array1D[i];
		}

		const vector4<T> & operator[](u32 i) const {
			return array2D[i];
		}

		void trans(const vector3<T> & vec) {
			identity();
			array2D[3][0] = vec[0];
			array2D[3][1] = vec[1];
			array2D[3][2] = vec[2];
		}

		matrix4x4<T> & operator=(const matrix4x4<T> & m) {
			for (int i = 0; i < 16; ++i) {
				array1D[i] = m.array1D[i];
			}
			return *this;
		}

		matrix4x4<T> inverse() {
			matrix4x4<T> inv_a;
			T buf;
			int i, j, k;
			inv_a.identity();
			for (i = 0; i < 4; ++i) {
				buf = 1.0f / array2D[i][i];
				for (j = 0; j < 4; ++j) {
					array2D[i][j] *= buf;
					inv_a[i][j] *= buf;
				}
				for (j = 0; j < 4; ++j) {
					if (i != j) {
						buf = array2D[j][i];
						for (k = 0; k < 4; ++k) {
							array2D[j][k] -= array2D[i][k] * buf;
							inv_a[j][k] -= inv_a[i][k] * buf;
						}
					}
				}
			}
			return inv_a;
		}
	};

	// C = AxB
	template<class T>
	matrix4x4<T> APICALL operator*(const matrix4x4<T> & r, const matrix4x4<T> & v) {
		matrix4x4<T> ret;
		ret.zero();
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				for (int k = 0; k < 4; ++k) {
					ret[i][j] += v[i][k] * r[k][j];
				}
			}
		}
		return ret;
	}

	template<class T>
	vector4<T> operator *(const vector4<T> & v, const matrix4x4<T> & m) {
		vector4<T> ret = { 0,0,0,0 };
		ret.x = v.x * m(0) + v.y * m(4) + v.z * m(8) + v.w * m(12);
		ret.y = v.x * m(1) + v.y * m(5) + v.z * m(9) + v.w * m(13);
		ret.z = v.x * m(2) + v.y * m(6) + v.z * m(10) + v.w * m(14);
		ret.w = v.x * m(3) + v.y * m(7) + v.z * m(11) + v.w * m(15);
		return ret;
	}

	typedef core::matrix4x4<f32> matrix;
	typedef core::vector3<f32> vec3;
	typedef core::vector4<f32> vec4;
}
