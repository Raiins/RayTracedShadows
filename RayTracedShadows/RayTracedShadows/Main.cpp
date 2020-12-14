#include "rtfinalproject.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include <chrono>
#include <iostream>

color ray_color(const ray& r, const hittable& world) {
    hit_record rec;
    if (world.hit(r, 0, infinity, rec)) {
        return 0.5 * (rec.normal + color(1, 1, 1));
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();


    // Image

    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 200;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;

    // World

    hittable_list world;

    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0));
    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), -0.45));
    world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5));


    // Camera

    camera cam(point3(-2, 2, 1), point3(0, 0, -1), vec3(0, 1, 0), 90, aspect_ratio);

    // Render

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    std::cerr << "Frame time in seconds:  " << (float) std::chrono::duration_cast<std::chrono::microseconds> (end - begin).count() / (float) 1000000 << std::endl;


    std::cerr << "\nDone.\n";

	system("PAUSE");
	return 0;
}