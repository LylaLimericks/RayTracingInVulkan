#include "RayTracingApplication.hpp"
#include <cstdlib>
#include <exception>
#include <iostream>

int main() {
  try {
    Implementations::RayTracingApplication app;
    app.run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}