#include "RayTracingApplication.hpp"
#include "vulkan/vulkan.hpp"
#include <iostream>
#include <ostream>
#include <stdexcept>

namespace RayTracing {

void RayTracingApplication::run() { initVulkan(); }

void RayTracingApplication::assignWindowManager(WindowManager &winManager) {
  windowManager = winManager;
}

void RayTracingApplication::initVulkan() { createInstance(); }

void RayTracingApplication::createInstance() {
  constexpr vk::ApplicationInfo appInfo{
      .pApplicationName = "Ray Tracer",
      .pEngineName = "No Engine",
      .engineVersion = VK_MAKE_VERSION(1, 0, 0),
      .apiVersion = vk::ApiVersion14,
  };

  std::vector<char const *> requiredLayers;
  if (enableValidationLayers) {
    requiredLayers.assign(validationLayers.begin(), validationLayers.end());
  }

  auto layerProperties = context.enumerateInstanceLayerProperties();
  if (std::ranges::any_of(
          requiredLayers, [&layerProperties](auto const &requiredLayer) {
            return std::ranges::none_of(
                layerProperties, [requiredLayer](auto const &layerProperty) {
                  return strcmp(layerProperty.layerName, requiredLayer) == 0;
                });
          })) {
    throw std::runtime_error(
        "One or more required Vulkan layers not supported!");
  }

  uint32_t extensionCount = 0;
  auto extensions = windowManager.getRequiredExtensions(extensionCount);

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

  instance = std::make_unique<vk::raii::Instance>(context, createInfo);
}

void RayTracingApplication::mainLoop() {
  std::cout << "Running the main loop..." << std::endl;
}

void RayTracingApplication::cleanup() {
  std::cout << "Cleaning Up..." << std::endl;
}

} // namespace RayTracing