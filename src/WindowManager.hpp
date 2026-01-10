#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include <GLFW/glfw3.h>
#include <cstdint>

namespace RayTracing {

class WindowManager {
public:
  const char **getRequiredExtensions(uint32_t &extensionCount);

  GLFWwindow *Handle() const { return window; }

private:
  GLFWwindow *window{};
};

} // namespace RayTracing

#endif