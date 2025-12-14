#include "RayTracingApplication.hpp"
#include "vulkan/vulkan.hpp"
#include <iostream>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <vulkan/vulkan_raii.hpp>

namespace RayTracing {

void RayTracingApplication::run() {
  initVulkan();
  mainLoop();
  cleanup();
}

void RayTracingApplication::assignWindowManager(WindowManager &winManager) {
  windowManager = winManager;
}

void RayTracingApplication::initVulkan() {
  createInstance();
  setupDebugMessenger();
}

void RayTracingApplication::createInstance() {
  constexpr vk::ApplicationInfo appInfo{
      .pApplicationName = "Ray Tracer",
      .pEngineName = "No Engine",
      .engineVersion = VK_MAKE_VERSION(1, 0, 0),
      .apiVersion = vk::ApiVersion14,
  };

  // Retrieve the list of required layers
  std::vector<char const *> requiredLayers;
  if (enableValidationLayers) {
    requiredLayers.assign(validationLayers.begin(), validationLayers.end());
  }

  // Ensure that the required layers are supported by the current Vulkan
  // implementation
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

  // Get the list of required Extensions
  auto extensions = getRequiredExtensions();

  // Ensure that the required extensions are supported by the Vulkan
  // implementation
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

void RayTracingApplication::setupDebugMessenger() {
  if (!enableValidationLayers)
    return;

  vk::DebugUtilsMessageSeverityFlagsEXT severityFlags(
      vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
      vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
      vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);
  vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags(
      vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
      vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
      vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);
  vk::DebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfoEXT{
      .messageSeverity = severityFlags,
      .messageType = messageTypeFlags,
      .pfnUserCallback = &RayTracingApplication::debugCallback,
  };

  debugMessenger = std::make_unique<vk::raii::DebugUtilsMessengerEXT>(
      instance->createDebugUtilsMessengerEXT(debugUtilsMessengerCreateInfoEXT));
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