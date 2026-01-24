#include "Vulkan/Surface.hpp"
#include "Vulkan/Instance.hpp"
#include "Vulkan/Window.hpp"
#include <vulkan/vulkan_raii.hpp>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>
namespace Vulkan {

// TODO: Verify and/or Refactor this to utilize appropriate ownership mechanisms with smart pointers
Surface::Surface(const class Instance &instance, const class Window &window) : instance(instance), window(window) {
  VkSurfaceKHR s;
  const auto ret = glfwCreateWindowSurface(instance.Handle(), window.Handle(), nullptr, &s);
  if (ret != 0) {
    throw std::runtime_error("Failed initializing a window surface!");
  }

  surface = instance.CreateSurface(s);
}

Surface::~Surface() = default;
} // namespace Vulkan