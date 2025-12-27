#include "Device.hpp"
#include "vulkan/vulkan.hpp"
#include <memory>
#include <vulkan/vulkan_raii.hpp>

namespace Vulkan {

Device::Device(vk::raii::PhysicalDevice physicalDevice,
               const std::vector<const char *> enabledExtensions,
               const vk::PhysicalDeviceFeatures &deviceFeatures,
               void *nextDeviceFeatures)
    : physicalDevice(
          std::make_unique<vk::raii::PhysicalDevice>(physicalDevice)) {

  const uint32_t computeFamilyIndex =
      findQueueFamily(vk::QueueFlagBits::eGraphics);

  const uint32_t graphicsFamilyIndex =
      findQueueFamily(vk::QueueFlagBits::eCompute);

  const float graphicsQueuePriority = 0.5f;

  vk::DeviceQueueCreateInfo deviceQueueCreateInfo{
      .queueFamilyIndex = graphicsFamilyIndex,
      .queueCount = 1,
      .pQueuePriorities = &graphicsQueuePriority,
  };

  vk::DeviceCreateInfo deviceCreateInfo{
      .pNext = nextDeviceFeatures,
      .queueCreateInfoCount = 1,
      .pQueueCreateInfos = &deviceQueueCreateInfo,
      .enabledExtensionCount = static_cast<uint32_t>(enabledExtensions.size()),
      .ppEnabledExtensionNames = enabledExtensions.data(),
      .pEnabledFeatures = &deviceFeatures,
  };

  const vk::raii::Device logicalDevice =
      vk::raii::Device(physicalDevice, deviceCreateInfo);

  this->logicalDevice =
      std::make_unique<vk::raii::Device>(physicalDevice, deviceCreateInfo);

  this->graphicsQueue =
      std::make_unique<vk::raii::Queue>(logicalDevice, graphicsFamilyIndex, 0);
}

/**
 * Finds the first queue family on the device that supports graphics
 *
 * @return Index of the queue family. -1 on none found.
 */
uint32_t Device::findQueueFamily(const vk::QueueFlags flags) {
  const std::vector<vk::QueueFamilyProperties> queueFamilyProperties =
      physicalDevice->getQueueFamilyProperties();

  for (uint32_t i = 0; i < queueFamilyProperties.size(); i++) {
    const auto qfp = queueFamilyProperties[i];
    if (qfp.queueFlags & flags) {
      return i;
    }
  }

  return -1;
}
} // namespace Vulkan