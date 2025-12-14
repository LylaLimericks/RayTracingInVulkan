#include "RayTracingApplication.hpp"
#include "vulkan/vulkan.hpp"
#include <iostream>
#include <ostream>
#include <stdexcept>

namespace RayTracing {

void RayTracingApplication::run() {
  initVulkan();
  mainLoop();
  cleanup();
}

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

  auto extensions = getRequiredExtensions();

  auto extensionProperties = context.enumerateInstanceExtensionProperties();

  for (const auto &extension : extensions) {
    if (std::ranges::none_of(
            extensionProperties, [extension](auto const &extensionProperty) {
              return strcmp(extensionProperty.extensionName, extension) == 0;
            })) {
      throw std::runtime_error("Required extension not supported: " +
                               std::string(extension));
    }
  }

  vk::InstanceCreateInfo createInfo{
      .pApplicationInfo = &appInfo,
      .enabledLayerCount = static_cast<uint32_t>(requiredLayers.size()),
      .ppEnabledLayerNames = requiredLayers.data(),
      .enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
      .ppEnabledExtensionNames = extensions.data(),
  };

  instance = std::make_unique<vk::raii::Instance>(context, createInfo);
}

std::vector<const char *> RayTracingApplication::getRequiredExtensions() {
  uint32_t extensionCount = 0;
  auto winExtensions = windowManager.getRequiredExtensions(extensionCount);

  std::vector extensions(winExtensions, winExtensions + extensionCount);

  if (enableValidationLayers) {
    extensions.push_back(vk::EXTDebugUtilsExtensionName);
  }

  return extensions;
}

void RayTracingApplication::mainLoop() {
  while (true)
    ;
  std::cout << "Running the main loop..." << std::endl;
}

void RayTracingApplication::cleanup() {
  std::cout << "Cleaning Up..." << std::endl;
}

} // namespace RayTracing