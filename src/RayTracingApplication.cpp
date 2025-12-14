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

  uint32_t extensionCount = 0;
  auto extensions = windowManager.getRequiredExtensions(&extensionCount);

  auto extensionProperties = context.enumerateInstanceExtensionProperties();

  for (uint32_t i = 0; i < extensionCount; ++i) {
    if (std::ranges::none_of(
            extensionProperties,
            [extension = extensions[i]](auto const &extensionProperty) {
              return strcmp(extensionProperty.extensionName, extension) == 0;
            })) {
      throw std::runtime_error("Required extension not supported: " +
                               std::string(extensions[i]));
    }
  }

  vk::InstanceCreateInfo createInfo{
      .pApplicationInfo = &appInfo,
      .enabledExtensionCount = extensionCount,
      .ppEnabledExtensionNames = extensions,
  };

  instance = vk::raii::Instance(context, createInfo);
}

void RayTracingApplication::mainLoop() {
  std::cout << "Running the main loop..." << std::endl;
}

void RayTracingApplication::cleanup() {
  std::cout << "Cleaning Up..." << std::endl;
}