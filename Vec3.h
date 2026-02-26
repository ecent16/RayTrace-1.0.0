#ifndef VEC3_H
#define VEC3_H

#include <ostream>

using std::sqrt;
using namespace std;

class Vec3 {
	/*
		Description: 
			Vec3 is a class used to store geometric vectors and colors. This classes focuses on three
			coordinates (x, y, z).
			Use 'double' or 'float' depending on how much memory you want to utilize. Most Ray Tracers use 'float'
	*/
	public:
		// Initialize array:
		float e[3];

		// Build Constructor:
		Vec3() : e{ 0, 0, 0 } {} // Best practice when instantiating an array
		Vec3(float e0, float e1,float e2) : e{e0, e1, e2} {}

		// Get functions for array:
		float x() const { return e[0]; }	// const here makes the function read only, and cannot be mutated
		float y() const { return e[1]; }
		float z() const { return e[2]; }
	
		//
		Vec3 operator - () const { return Vec3(-e[0], -e[1], -e[2]); }
		float operator[] (int i) const { return e[i]; }
		float& operator[] (int i) { return e[i]; }

		Vec3& operator += (const Vec3& v) {
			e[0] += v.e[0], e[1] += v.e[1], e[2] += v.e[2];
			return *this;
		}

		Vec3& operator *= (float t) {
			e[0] *= t, e[1] *= t, e[2] *= t;
			return *this;
		}

		Vec3& operator /= (float t) { return *this *= 1 / t; }

		float length_squared() const { 
			return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; 
		}

		bool near_zero() const {
			// Return true if the vector is close to zero in all dimensions:
			auto s = 1e-8;
			return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
		}

		float length() const { return sqrt(length_squared()); }

		static Vec3 random() {
			return Vec3(random_float(), random_float(), random_float());
		}

		static Vec3 random(float min, float max) {
			return Vec3(random_float(min, max), random_float(min, max), random_float(min, max));
		}

};

// point3 is an alias for vec3Class, meant for geometric clarity in code:
using point3 = Vec3;

// Vector utility funtions:
/*
	Inline Functions helps reduce the function call overhead. Inline function gets inserted/substituted at the point of the 
	inline function call. Inline is ideal for short functions that return a value.
*/
inline ostream& operator << (ostream& out, const Vec3& v) {
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline Vec3 operator + (const Vec3& u, const Vec3& v) {
	return Vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline Vec3 operator - (const Vec3& u, const Vec3& v) {
	return Vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline Vec3 operator * (const Vec3& u, const Vec3& v) {
	return Vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline Vec3 operator * (float t, const Vec3& v) {
	return Vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline Vec3 operator * (const Vec3& v, float t) {
	return t * v;
}

inline Vec3 operator / (const Vec3& v, float t) {
	return (1 / t) * v;
}

inline float dot(const Vec3& u, const Vec3& v) {
	return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

inline Vec3 cross(const Vec3& u, const Vec3& v) {
	return Vec3(
		u.e[1] * v.e[2] - u.e[2] * v.e[1],
		u.e[2] * v.e[0] - u.e[0] * v.e[2],
		u.e[0] * v.e[1] - u.e[1] * v.e[0]
	);
}

// Used for ray_color functions:
/*
	Purpose: 
		These functions will assist in diffusing materials such as filtering
		vectors that hit an object to give it a more realistic appearance by manipulating 
		random vectors.
*/
inline Vec3 unit_vector(const Vec3& v) {
	return v / v.length();
}

inline Vec3 random_in_unit_disk() {
	// Used for Defocusing camera:
	while (true) {
		auto p = Vec3(random_float(-1, 1), random_float(-1, 1), 0);
		if (p.length_squared() < 1) { return p; }
	}
}

inline Vec3 random_unit_vector() {
	while (true) {
		auto point = Vec3::random(-1, 1);
		auto len_sqrd = point.length_squared();
		if(1e-160 < len_sqrd && len_sqrd <= 1) { return point / sqrt(len_sqrd); }
	}
}

inline Vec3 random_on_hemisphere(const Vec3& normal) {
	// Indicate whether a vector is on the hemisphere of an object (indicated as a positive number:
	Vec3 on_unit_sphere = random_unit_vector();
	if (dot(on_unit_sphere, normal) > 0.0) { return on_unit_sphere; }
	else { -on_unit_sphere; }
}

inline Vec3 reflect(const Vec3& vector, const Vec3& n) {
	return vector - 2 * dot(vector, n) * n;
}

inline Vec3 refract(const Vec3& uv, const Vec3& normal, float etai_over_etat) {
	auto cos_theta = fmin(dot(-uv, normal), 1.0);
	Vec3 ray_out_perp = etai_over_etat * (uv + cos_theta * normal);
	Vec3 ray_out_parallel = -sqrt( fabs(1.0 - ray_out_perp.length_squared()) ) * normal;

	return ray_out_perp + ray_out_parallel;
}

#endif