#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "Hittable.h"
#include <vector>

/*
	Notes:
	- shared_ptr<type> is a pointer to some allocated type. Its a pointer that can be incremented (or decremented) where if
		the pointer drops down to 0, the pointer will be deleted safely.
		Ex: shared_ptr<double> double_ptr = make_shared<double>
	- shared_ptr is always used with <memory> to automate and manage memory more easily.
	- vector is a generic array-like collection of an arbitrary type. 
*/

using namespace std;
class Hittable_List : public Hittable {
	// This class will store Hittable object data:
	public:
		std::vector <shared_ptr <Hittable> > objects;

		Hittable_List() {}
		Hittable_List(shared_ptr <Hittable> object) { add(object); }

		void clear() { objects.clear(); }

		void add(shared_ptr <Hittable> object) {
			objects.push_back(object);
		}

		// Initiate Interval class (min and max):
		bool hit(const Ray& ray, Interval ray_t, Hit_Record& record) const override {
			Hit_Record temp_record;
			bool hit_anything = false;
			auto closest_point = ray_t.max;

			for (const auto& object : objects) {
				if (object->hit(ray, Interval(ray_t.min, closest_point), temp_record)) {
					hit_anything = true;
					closest_point = temp_record.t;
					record = temp_record;
				}
			}
			return hit_anything;
		}
};


#endif
