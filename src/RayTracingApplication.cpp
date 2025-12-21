#include "RayTracingApplication.hpp"
#include "Vulkan/Device.hpp"
#include "vulkan/vulkan.hpp"
#include <iostream>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <vulkan/vulkan_core.h>
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

void RayTracingApplication::pickPhysicalDevice() {
  auto devices = instance->enumeratePhysicalDevices();
  if (devices.empty()) {
    throw std::runtime_error("Failed to find GPUs with Vulkan Support!");
  }

  // Iterate through the devices to find a suitable candidate
  for (const auto &device : devices) {
    const auto queueFamilies = device.getQueueFamilyProperties();
    bool isSuitable = device.getProperties().apiVersion >= VK_API_VERSION_1_3;

    if (!isSuitable) {
      continue;
    }

    // Verify the existence of a queue family that supports graphics
    for (const auto &queueFamilyProps : queueFamilies) {
      if ((queueFamilyProps.queueFlags & vk::QueueFlagBits::eGraphics) !=
          static_cast<vk::QueueFlags>(0)) {
        isSuitable = false;
        break;
      }
    }

    // Vertify that the device supports all required extensions
    auto extensions = device.enumerateDeviceExtensionProperties();
    bool foundAllExtensions = true;
    for (const auto &reqExtension : deviceExtensions) {
      bool foundExtension = false;
      for (const auto &extension : extensions) {
        if (strcmp(extension.extensionName, reqExtension) == 0) {
          foundExtension = true;
          break;
        }
      }
      foundAllExtensions = foundAllExtensions & foundExtension;
    }

    isSuitable = isSuitable && foundAllExtensions;
    if (isSuitable) {
      this->device = Vulkan::Device(device, deviceExtensions);
      return;
    }
  }

  throw std::runtime_error("Failed to find a suitable GPU!");
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