#ifndef WINDOW_H
#define WINDOW_H

#include "Vulkan/WindowConfig.hpp"
#include <GLFW/glfw3.h>
#include <vector>
namespace Vulkan {
class Window {
public:
  explicit Window(const WindowConfig &config);

  const WindowConfig &Config() const { return windowConfig; }
  const std::vector<const char *> GetRequiredExtensions() const;

private:
  const WindowConfig windowConfig;
  const GLFWwindow *window;
};
} // namespace Vulkan

#endif