#ifndef HITTABLE_H
#define HITTABLE_H

using namespace std;
class Material;

class Hit_Record {
	// Utilize Ray class variables:
	// A class that represents any object that is hit by a ray:
	// Class also calculates the normal (front facing) side of an object:
	public:
		point3 p;
		Vec3 normal;
		shared_ptr<Material> mat;
		float t = 0.0;
		bool front_face = true;

		void set_face_normal(const Ray& ray, const Vec3& outward_norm) {
			// Sets the hit record normal vector:
			// NOTE: The parameter 'outward_norm' is assumed to have a unit length.
			front_face = dot(ray.direction(), outward_norm) < 0;
			normal = front_face ? outward_norm : -outward_norm;
		}
};

class Hittable {
	/*
		Notes:
		- virtual functions are a member function that is declared within a base class (example below) and is re-defined
			(overriden) by a derived class (in this case Interval and Ray).
		- virtual functions are used for runtime Polymorphism, and to ensure the correct function is called for an object.
	*/
	public:
		virtual ~Hittable() = default;

		virtual bool hit(const Ray& ray, Interval ray_t, Hit_Record& record) const = 0;

};

#endif
