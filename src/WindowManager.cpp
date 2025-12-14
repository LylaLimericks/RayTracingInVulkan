#include "WindowManager.hpp"
#define GLFW_INCLUDE_VULKAN // REQUIRED only for GLFW CreateWindowSurface.
#include <GLFW/glfw3.h>

namespace RayTracing {

const char **WindowManager::getRequiredExtensions(uint32_t &extensionCount) {
  return glfwGetRequiredInstanceExtensions(&extensionCount);
}

} // namespace RayTracing