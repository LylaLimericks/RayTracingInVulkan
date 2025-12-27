#include "Application.hpp"
#include "Vulkan/Device.hpp"
#include "Window.hpp"
#include "vulkan/vulkan.hpp"
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_raii.hpp>

namespace {

vk::raii::PhysicalDevice pickPhysicalDevice(const std::vector<vk::raii::PhysicalDevice> &devices, const std::vector<const char *> requiredExtensions) {
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
    for (const auto &reqExtension : requiredExtensions) {
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
      return device;
    }
  }

  throw std::runtime_error("Failed to find a suitable GPU!"); // TODO: Convert this over to no longer rely on exception handling.
}

} // namespace

namespace Vulkan {

Application::Application(const ApplicationInfo &appInfo,
                         const WindowConfig &windowConfig,
                         VkPresentModeKHR presentMode,
                         bool enableValidationLayers) {
  const auto validationLayers =
      enableValidationLayers
          ? std::vector<const char *>{"VK_LAYER_KHRONOS_validation"}
          : std::vector<const char *>();

  window = std::make_unique<Window>(windowConfig);
  pickDefaultPhysicalDevice();
}

void Application::pickDefaultPhysicalDevice() {
  const auto devices = instance->PhysicalDevices();

  const std::vector<const char *> deviceExtensions{
      vk::KHRSwapchainExtensionName,
      vk::KHRSpirv14ExtensionName,
      vk::KHRSynchronization2ExtensionName,
      vk::KHRCreateRenderpass2ExtensionName,
  };
  const auto pickedDevice = pickPhysicalDevice(devices, deviceExtensions);

  vk::StructureChain<vk::PhysicalDeviceFeatures2,
                     vk::PhysicalDeviceVulkan13Features,
                     vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>
      featureChain = {
          {}, // vk::PhysicalDeviceFeatures2 (leaving empty for now)
          {.dynamicRendering =
               true}, // Enable dynamic rendering from Vulkan 1.3
          {.extendedDynamicState =
               true} // Enable extended dynamic state from the extension.
      };

  const vk::PhysicalDeviceFeatures deviceFeatures{};
  this->setDevice(
      std::make_unique<Device>(
          pickedDevice,
          deviceExtensions,
          deviceFeatures,
          &featureChain));
}

void Application::setDevice(std::unique_ptr<Device> device) {
  this->device = std::move(device);
}

} // namespace Vulkan