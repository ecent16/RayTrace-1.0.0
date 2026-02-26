/*
	FileName: RayTraceMain.cpp
	CreatedBy: Ervin Centeno
	Description:
	- Main Script for executing Ray Trace.
    - Vec3 is a class used to store geometric vectors and colors. This classes focuses on three coordinates (x, y, z).
    - Use 'double' or 'float' depending on how much memory you want to utilize. Most Ray Tracers use 'float'.
    - Ray classes are need for computing what colors are seen along a ray.
    - Run command 'cmake --build RayTrace-Build --config release' to check for errors.

    - For cmake, use the command 'cmake -B RayTrace-Build' to create a new Build folder. Ensure that the CMAke file is in the same location.
    - Use 'cmake --build RayTrace-Build' to create an executable file in the new Build folder. 
    - Use the command 'RayTrace-Build\Debug\RayTrace.exe > image.ppm' to export ppm images.

*/
#include "RayTrace_MainHeader.h"

#include "Camera.h"
#include "Hittable.h"
#include "Hittable_List.h"
#include "Material.h"
#include "Sphere.h"

// Function Prototypes:

int main() {

    Hittable_List world;

    auto ground = make_shared<Lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(point3(0, -1000, 0), 1000, ground));

    // Create random spheres within the world:
    for (int a = -8; a < 8; a++) {
        for (int b = -8; b < 8; b++) {
            auto choose_mat = random_float();
            point3 center(a + 0.9 * random_float(), 0.2, b + 0.9 * random_float());

            if ((center = point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // Diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<Lambertian>(albedo);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95) {
                    // Metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_float(0, 0.5);
                    sphere_material = make_shared<Metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else {
                    // Glass:
                    sphere_material = make_shared<Dielectric>(1.5);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }
    
    // Additional Spheres:
    auto sphere1_mat = make_shared<Dielectric>(1.5);
    world.add(make_shared<Sphere>(point3(0, 1, 0), 1.0, sphere1_mat));

    auto sphere2_mat = make_shared<Lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(point3(-4, 1, 0), 0.1, sphere2_mat));

    auto sphere3_mat = make_shared<Metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(point3(4, 1, 0), 1.0, sphere3_mat));

    // Camera position and settings:
    Camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.img_width = 1080;
    cam.samples_per_px = 500;
    cam.max_depth = 50;

    cam.vFoV = 20;
    cam.look_from = point3(13, 2, 3);
    cam.look_at = point3(0, 0, 0);
    cam.v_up = Vec3(0, 1, 0);

    cam.defocus_angle = 0.6;
    cam.focus_distance = 10.0;

    cam.render(world);

	return 0;
}

/*
    auto material_ground = make_shared<Lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<Lambertian>(color(0.1, 0.2, 0.5));

    // Fuzz effect has been added for metal objects:
    auto material_left = make_shared<Dielectric>(1.50);
    auto material_bubble = make_shared<Dielectric>(1.00 / 1.50);

    auto material_right = make_shared<Metal>(color(0.8, 0.6, 0.2), 1.0);

    // Add objects to the world, set positions and size:
    world.add(make_shared<Sphere>(point3(0.0, -100.5, -1), 100.0, material_ground));
    world.add(make_shared<Sphere>(point3(0.0, 0.0, -1.2), 0.5, material_center));
    world.add(make_shared<Sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<Sphere>(point3(-1.0, 0.0, -1.0), 0.4, material_bubble));
    world.add(make_shared<Sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));
*/

