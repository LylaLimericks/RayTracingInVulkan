#include "Vulkan/Window.hpp"
#include "Vulkan/WindowConfig.hpp"
#include "vulkan/vulkan.hpp"
#include <GLFW/glfw3.h>
#include <cstdint>
#include <stdexcept>
namespace Vulkan {
Window::Window(const WindowConfig &config) : windowConfig(config) {

  if (!glfwInit()) {
    throw(std::runtime_error("glfw failed to initialize!"));
  }

  if (!glfwVulkanSupported()) {
    throw(std::runtime_error("glfw doesn't support Vulkan!"));
  }

  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  window = glfwCreateWindow(config.Width, config.Height, config.Title.c_str(),
                            nullptr, nullptr);

  if (window == nullptr) {
    throw(std::runtime_error("Failed to create the window!"));
  }
}

const std::vector<const char *> Window::GetRequiredExtensions() const {
  uint32_t extensionCount = 0;
  const char **extensions = glfwGetRequiredInstanceExtensions(&extensionCount);

  return std::vector<const char *>(extensions, extensions + extensionCount);
}

vk::Extent2D Window::FramebufferSize() const {
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);

  return vk::Extent2D{
      .width = static_cast<uint32_t>(width),
      .height = static_cast<uint32_t>(height),
  };
}

bool Window::shouldClose() const {
  return !glfwWindowShouldClose(window);
}

void Window::pollEvents() const {
  glfwPollEvents();
}

} // namespace Vulkan