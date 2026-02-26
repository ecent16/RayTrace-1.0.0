#ifndef SPHERE_H
#define SPHERE_H

#include "Hittable.h"

using namespace std;
class Sphere : public Hittable {
	private:
		point3 center;
		float radius;
		shared_ptr<Material> mat;

	public:
		// Initialize the material pointer 'mat':
		Sphere(const point3& center, float radius, shared_ptr<Material> mat) :
			center(center), radius(fmax(0, radius)), mat(mat) {}
		

		bool hit(const Ray& ray, Interval ray_t, Hit_Record& record) const override {
			// Use the Spheres location to determine normals:
			Vec3 orig_center = center - ray.origin();
			auto a = ray.direction().length_squared();
			auto h = dot(ray.direction(), orig_center);
			auto c = orig_center.length_squared() - radius * radius;

			auto discriminant = h * h - a * c;
			if (discriminant < 0) { return false; }
			
			auto sqrtd = sqrt(discriminant);

			// Find the nearest root that lies in the acceptable range:
			auto root = (h - sqrtd) / a;
			if (!ray_t.surrounds(root)) {
				root = (h + sqrtd) / a;
				if (!ray_t.surrounds(root)) { return false; }
			}

			record.t = root;
			record.p = ray.at(record.t);
			Vec3 outward_norm = (record.p - center) / radius;
			record.set_face_normal(ray, outward_norm);
			record.mat = mat;

			return true;

		}
};

#endif
