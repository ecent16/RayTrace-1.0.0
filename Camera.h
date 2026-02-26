#ifndef CAMERA_H
#define CAMERA_H

#include "Hittable.h"
#include "Interval.h"
#include "Material.h"

using namespace std;
class Camera {

    public:
        // Set Image Parameters using width and aspect ratio:
        float aspect_ratio = 1.0;
        int img_width = 100;
        int samples_per_px = 10;
        int max_depth = 10;

        float vFoV = 90; // Vertical Field of View (vFoV)
        point3 look_from = point3(0, 0, 0);
        point3 look_at = point3(0, 0, -1);
        Vec3 v_up = Vec3(0, 1, 0);

        float defocus_angle = 0;
        float focus_distance = 10;

        void render(const Hittable& world) {
            init();
            
            // Pixels are written in rows, written left to right, top to bottom.
            cout << "P3\n" << img_width << " " << img_height << "\n255\n";
            
            for (int j = 0; j < img_height; j++) {
                // Progress indicator for render:
                clog << "\rScanlines remaining: " << (img_height - j) << ' ' << flush;

                for (int i = 0; i < img_width; i++) {
                    
                    color px_color(0, 0, 0);
                    for (int sample = 0; sample < samples_per_px; sample++) {
                        Ray ray = get_ray(i, j);
                        px_color += ray_color(ray, max_depth, world);
                    }
                    write_color(cout, px_samples_scale * px_color);
                    
                }
            }

            clog << "\rDone! \n";
            
        }

	private:
        int img_height;
        float px_samples_scale;

        point3 camera_center;
        point3 px_location;
        Vec3 px_delta_h;
        Vec3 px_delta_v;
        Vec3 u, v, w;           // Camera axis' relactive to the the direction the camera is facing: 
        Vec3 defocus_disk_h;    // Defocus disk horizontal redius
        Vec3 defocus_disk_v;    // Defocus disk vertical redius

		void init() {

            img_height = int(img_width / aspect_ratio);
            img_height = (img_height < 1) ? 1 : img_height;

            px_samples_scale = float(1.0) / samples_per_px;

            camera_center = look_from;
       
            // Adjust viewport (camera) position:
            auto theta = degrees_to_radians(vFoV);
            auto h = tan(theta / 2);
            auto vp_height = 2 * h * focus_distance;
            auto vp_width = vp_height * (float(img_width) / img_height);

            // Calculate u, v, w unit basis vectors for the camera coordinate frame:
            w = unit_vector(look_from - look_at);
            u = unit_vector(cross(v_up, w));
            v = cross(w, u);

            // Calculate the vectors across the horizonal(u) and down the vertical(v) viewport edges:
            auto viewport_h = vp_width * u;     // Vector across viewport horizontal edge
            auto viewport_v = vp_height * -v;   // Vector down viewport vertical edge

            // Calculate the horizontal and vertical delta vectors from pixel to pixel:
            px_delta_h = viewport_h / img_width;
            px_delta_v = viewport_v / img_height;

            // Calculate the location of the upper left pixel:
            auto vp_upperLeft = camera_center - (focus_distance * w) - viewport_h / 2 - viewport_v / 2;

            px_location = vp_upperLeft + 0.5 * (px_delta_h + px_delta_v); 

            // Calculate the camera defocus disk basis vector:
            auto defocus_radius = focus_distance * tan(degrees_to_radians(defocus_angle / 2));
            defocus_disk_h = u * defocus_radius;
            defocus_disk_v = v * defocus_radius;
		}

        Ray get_ray(int i, int j) const {
            // Construct a camera ray orientating from the defocus disk and directed at a 
            // randomly sampled point around the pixel location i and j
            auto offset = sample_square();
            auto px_sample = px_location + ((i + offset.x()) * px_delta_h) + ((j + offset.y()) * px_delta_v);

            auto ray_origin = (defocus_angle <= 0) ? camera_center : defocus_disk_sample();
            auto ray_direction = px_sample - ray_origin;

            return Ray(ray_origin, ray_direction);
        }

		color ray_color(const Ray& ray, int depth, const Hittable& world) const {
            /*
               Description:
               - This function will blend colors in a gradient.
               - This will blend colors based on the Y coordinate after scaling the ray direction.
               - Linear Scale: 0.0 <= a <= 1.0
               - BlendValue = (1 -a) * startValue + a * endValue
               - To fix shadow acne (floating points being intersected below the object surface), by offseting
                 the hit location of the ray.
               - Images should be stored in gamma space.
           */
            if (depth <= 0) { return color(0, 0, 0); }

            Hit_Record record;
            if (world.hit(ray, Interval(0.001, infinity), record)) {
                Ray scattered;
                color attenuation;
                if(record.mat -> scatter(ray, record, attenuation, scattered)) { 
                    return attenuation * ray_color(scattered, depth - 1, world);
                }

                return color(0, 0, 0);
            }

            // Set ray direction:
            Vec3 unit_direction = unit_vector(ray.direction());
            auto a = 0.5 * (unit_direction.y() + 1.0);

            // Return the blend value for the gradient:
            return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
		}

        Vec3 sample_square() const {
            // Returns the vector to random point in the [-0.5, -0.5] - [+0.5, +0.5] unit square:
            float const unit_scale = 0.5;
            return Vec3(random_float() - unit_scale, random_float() - unit_scale, 0);
        }

        point3 defocus_disk_sample() const {
            // Return a random point in the camera defocus disk:
            auto p = random_in_unit_disk();
            return camera_center + (p[0] * defocus_disk_h) + (p[1] * defocus_disk_v);
        }
};

#endif
