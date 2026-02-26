#ifndef MATERIAL_H
#define MATERIAL_H

#include "Hittable.h"
#include "Ray.h"
#include "Color.h"

/*
	Notes:
	- Glass objects, water, and diamonds are dielectrics. Meaning when light hits them, it splits rays into a reflected ray and a 
		refracted (transmitted) ray. Refracted rays bend as it transitions from a materials surroundings into the material itself.
*/
class Material {
	public:
		virtual ~Material() = default;

		virtual bool scatter(
			const Ray& ray_in, const Hit_Record& record, color& attenuation, Ray& scattered
		) const { return false; }
};

class Lambertian : public Material {
	public:
		// Lambertian means diffuse. Gives objects a matte appearance:
		Lambertian(const color& albedo) : albedo(albedo) {}

		bool scatter(const Ray& ray_in, const Hit_Record& record, color& attenuation, Ray& scattered) 
		const override {
			auto scatter_direction = record.normal + random_unit_vector();

			// Catch degenerate scatter direction:
			if (scatter_direction.near_zero()) {
				scatter_direction = record.normal;
			}

			scattered = Ray(record.p, scatter_direction);
			attenuation = albedo;
			return true;
		}

	private:
		// Albedo means 'Whiteness' in latin:
		color albedo;
};

class Metal : public Material {
	public:
		Metal(const color& albedo, float fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

		// The override will take place of the Material scatter function:
		bool scatter(const Ray& ray_in, const Hit_Record& record, color& attenuation, Ray& scattered) 
		const override {
			Vec3 reflected = reflect(ray_in.direction(), record.normal);

			// Add fuzz effect to a metal sphere:
			reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
			scattered = Ray(record.p, reflected);
			attenuation = albedo;
			return (dot(scattered.direction(), record.normal) > 0);
		}

	private:
		color albedo;
		float fuzz;
};

class Dielectric : public Material {
	public:
		Dielectric(float refract_index) : refract_index(refract_index) {}

		bool scatter(const Ray& ray_in, const Hit_Record& record, color& attenuation, Ray& scattered) 
		const override {
			attenuation = color(1.0, 1.0, 1.0);
			float ri = record.front_face ? (1.0 / refract_index) : refract_index;

			Vec3 unit_direction = unit_vector(ray_in.direction());

			// Calculate the total internal reflection of the object:
			float cos_theta = fmin(dot(-unit_direction, record.normal), 1.0);
			float sin_theta = sqrt(1.0 - cos_theta * cos_theta);

			bool cannot_refract = (ri * sin_theta) > 1.0;

			Vec3 direction;
			direction = (cannot_refract || reflectance(cos_theta, ri) > random_float()) ?
				reflect(unit_direction, record.normal) :  refract(unit_direction, record.normal, ri); 

			scattered = Ray(record.p, direction);
			return true;
		}

	private:
		// Refractive index in vacuum or air, or the ratio of the material's refractive index over the refractive index of the enclosing media.
		// Utlizes the Snell's Law:
		float refract_index;

		static float reflectance(float cosine, float refract_index) {
			// Use Schlick's Approximation for reflectance:
			auto r0 = (1 - refract_index) / (1 + refract_index);
			r0 = r0 * r0;

			return r0 + (1 - r0) * pow((1 - cosine), 5);
		}
};
#endif
