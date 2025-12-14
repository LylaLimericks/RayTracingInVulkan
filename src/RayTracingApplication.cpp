#include "RayTracingApplication.hpp"
#include "vulkan/vulkan.hpp"
#include <iostream>
#include <ostream>
#include <stdexcept>

#define GLFW_INCLUDE_VULKAN // REQUIRED only for GLFW CreateWindowSurface.
#include <GLFW/glfw3.h>

void RayTracingApplication::run() { std::cout << "Running!" << std::endl; }

void RayTracingApplication::assignWindowManager(WindowManager &winManager) {
  windowManager = winManager;
}

void RayTracingApplication::initVulkan() {
  std::cout << "Intializing Vulkan..." << std::endl;
}

void RayTracingApplication::createInstance() {
  constexpr vk::ApplicationInfo appInfo{
      .pApplicationName = "Ray Tracer",
      .pEngineName = "No Engine",
      .engineVersion = VK_MAKE_VERSION(1, 0, 0),
      .apiVersion = vk::ApiVersion14,
  };

  uint32_t glfwExtensionCount = 0;
  auto glfwExtensions =
      windowManager.getRequiredExtensions(&glfwExtensionCount);

  auto extensionProperties = context.enumerateInstanceExtensionProperties();

  for (uint32_t i = 0; i < glfwExtensionCount; ++i) {
    if (std::ranges::none_of(
            extensionProperties,
            [glfwExtension = glfwExtensions[i]](auto const &extensionProperty) {
              return strcmp(extensionProperty.extensionName, glfwExtension) ==
                     0;
            })) {
      throw std::runtime_error("Required GLFW extension not supported: " +
                               std::string(glfwExtensions[i]));
    }
  }

  vk::InstanceCreateInfo createInfo{
      .pApplicationInfo = &appInfo,
  };
}

void RayTracingApplication::mainLoop() {
  std::cout << "Running the main loop..." << std::endl;
}

void RayTracingApplication::cleanup() {
  std::cout << "Cleaning Up..." << std::endl;
}