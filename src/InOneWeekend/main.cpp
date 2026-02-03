#include "Vulkan/application.hpp"
#include "camera.h"
#include "hittableList.h"
#include "sphere.h"
#include "vec3.h"

#include <iostream>

int main() {
  Vulkan::Application app;

  hittableList world;

  auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
  auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
  auto material_left = make_shared<dielectric>(1.50);
  auto material_bubble = make_shared<dielectric>(1.00 / 1.50);
  auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

  world.add(
      make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
  world.add(make_shared<sphere>(point3(0.0, 0.0, -1.2), 0.5, material_center));
  world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
  world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.4, material_bubble));
  world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

  camera cam;

  cam.aspectRatio = 16.0 / 9.0;
  cam.imageWidth = 400;
  cam.samplesPerPixel = 100;
  cam.maxDepth = 50;

  cam.vFov = 20;
  cam.lookFrom = point3(-2, 2, 1);
  cam.lookAt = point3(0, 0, -1);
  cam.vUp = vec3(0, 1, 0);

  cam.render(world);

  app.mainLoop();
  return 0;
}