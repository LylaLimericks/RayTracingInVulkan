#include "camera.h"
#include "hittableList.h"
#include "sphere.h"
#include "vec3.h"

#include <iostream>

color rayColor(const ray &r, const hittable &world)
{
}

int main()
{
    hittableList world;

    auto R = std::cos(pi / 4);

    auto material_left = make_shared<lambertian>(color(0, 0, 1));
    auto material_right = make_shared<lambertian>(color(1, 0, 0));

    world.add(make_shared<sphere>(point3(-R, 0, -1), R, material_left));
    world.add(make_shared<sphere>(point3(R, 0, -1), R, material_right));

    camera cam;

    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;

    cam.vFov = 90;

    cam.render(world);

    return 0;
}