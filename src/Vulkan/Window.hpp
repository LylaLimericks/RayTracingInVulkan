#ifndef WINDOW_H
#define WINDOW_H

#include "Vulkan/WindowConfig.hpp"
#include "vulkan/vulkan.hpp"
#include <GLFW/glfw3.h>
#include <vector>
namespace Vulkan {
class Window {
public:
  explicit Window(const WindowConfig &config);

  const WindowConfig &Config() const { return windowConfig; }
  const std::vector<const char *> GetRequiredExtensions() const;
  bool shouldClose() const;
  void pollEvents() const;

  GLFWwindow *Handle() const { return window; }
  vk::Extent2D FramebufferSize() const;

private:
  const WindowConfig windowConfig;
  GLFWwindow *window;
};
} // namespace Vulkan

#endif