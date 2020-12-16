#include "rtfinalproject.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include <chrono>
#include <iostream>

//center of the light source
const point3 LIGHTSOURCE = point3(0.0, 3.0, 0.0);
//Set soft shadows or hard shadows
const bool OPTIMIZE = true;
//Sets radius for light
const auto OPTIMIZE_RADIUS = 0.3;
//Image settings
const auto ASPECT_RATIO = 3.0 / 2.0;
const int IMAGE_WIDTH = 1200;
//Sphere Gen settings
const auto UPPERLIMIT = 1.8;

//Function that generates the random point for the light source
point3 randomizeLightSource(point3 source, double radius) {
    //Random point3 within radius
    double x = random_double(0, radius);
    double y = random_double(0, radius);
    double z = random_double(0, radius);
    //returns the random point + source to translate the random point to the right pos
    return point3(x, y, z) + source;
}

hittable_list random_scene() {
    hittable_list world;

    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, randomColor()));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            //Only change is this check here to include the upperlimit because this was taking way to long to render for testing with the unbounded upperlimit
            if ((center - point3(4, 0.2, 0)).length() > 0.9 && (center - point3(4, 0.2, 0)).length() < UPPERLIMIT) {
                world.add(make_shared<sphere>(center, 0.2));
            }
        }
    }

    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, randomColor()));

    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, randomColor()));

    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, randomColor()));

    return world;
}


color ray_color(const ray& r, const hittable& world) {
    //Class that records the points that are hit
    hit_record rec, rec2;
    //If a cast ray hits something in the world
    if (world.hit(r, 0.001, infinity, rec)) {
        //check which type of shadow is desired (this is hard shadow)
        if (OPTIMIZE == false) {
            //If the ray between the light and the point that was originally hit hits something else then...
            if (world.hit(ray(rec.p, LIGHTSOURCE - rec.p), 0.001, 100, rec2))
            {
                //make that pixel darker
                return color(0.0, 0.0, 0.0);
            }
            //if not just return the desired color of the sphere
            else {
                return rec.color_ptr;
            }
        }
        //check which type of shadow is desired (this is soft shadow)
        else if (OPTIMIZE == true) {
            //If the ray between the random light point and the point that was originally hit hits something else then...
            if (world.hit(ray(rec.p, randomizeLightSource(LIGHTSOURCE, OPTIMIZE_RADIUS) - rec.p), 0.001, 100, rec2))
            {
                //make that pixel darker
                return color(0.0, 0.0, 0.0);
            }
            //if not just return the desired color of the sphere
            else {
                return rec.color_ptr;
            }
        }
    }

    //Setting the background color
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}


//https://www.geeksforgeeks.org/converting-seconds-into-days-hours-minutes-and-seconds/  
// function convert second into day  
// hours, minutes and seconds 
void ConvertSectoDay(int n)
{
    int day = n / (24 * 3600);

    n = n % (24 * 3600);
    int hour = n / 3600;

    n %= 3600;
    int minutes = n / 60;

    n %= 60;
    int seconds = n;

    //Outputs frametimer that records how long it took to render:
    std::cerr << std::endl << "Frame time: " << day << " " << "days " << hour
        << " " << "hours " << minutes << " "
        << "minutes " << seconds << " "
        << "seconds " << std::endl;
}


//Had to change some stuff for the camera and the global variables
int main() {
    //https://stackoverflow.com/a/27739925 
    //Start Timer
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    // Image

    const int image_height = static_cast<int>(IMAGE_WIDTH / ASPECT_RATIO);
    const int samples_per_pixel = 100;

    // World


    hittable_list world = random_scene();

    // Camera

    point3 lookfrom(13, 2, 3);
    point3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    camera cam(lookfrom, lookat, vup, 20, ASPECT_RATIO, aperture, dist_to_focus);

    // Render

    std::cout << "P3\n" << IMAGE_WIDTH << " " << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j  << ' ' << "Out of " << image_height << " total scanlines."<< std::flush;
        for (int i = 0; i < IMAGE_WIDTH; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (IMAGE_WIDTH - 1);
                auto v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    //https://stackoverflow.com/a/27739925 
    //end timer
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    //Uses convert funtion to change it into 
    ConvertSectoDay(std::chrono::duration_cast<std::chrono::seconds> (end - begin).count());

    std::cerr << "\nDone.\n";

	system("PAUSE");
	return 0;
}
