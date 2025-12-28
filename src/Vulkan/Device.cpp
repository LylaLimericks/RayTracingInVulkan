#include "Device.hpp"
#include "vulkan/vulkan.hpp"
#include <memory>
#include <stdexcept>
#include <vulkan/vulkan_raii.hpp>

namespace {
/**
 * Finds the first queue family on the device which supports the appropriate flags
 *
 * @return Index of the queue family. -1 on none found.
 */
uint32_t findQueueFamily(const std::vector<vk::QueueFamilyProperties> &queueFamilyProperties, const vk::QueueFlags flags) {
  for (uint32_t i = 0; i < queueFamilyProperties.size(); i++) {
    const auto qfp = queueFamilyProperties[i];
    if (qfp.queueFlags & flags) {
      return i;
    }
  }

  return -1;
}

} // namespace

namespace Vulkan {

Device::Device(vk::raii::PhysicalDevice physicalDevice,
               const Surface &surface,
               const std::vector<const char *> enabledExtensions,
               const vk::PhysicalDeviceFeatures &deviceFeatures,
               void *nextDeviceFeatures)
    : physicalDevice(
          std::make_unique<vk::raii::PhysicalDevice>(physicalDevice)) {

  // Find which queues we want to utilize for what.

  const auto queueFamilies = physicalDevice.getQueueFamilyProperties();

  auto graphicsQueueFamilyProperties = std::ranges::find_if(
      queueFamilies,
      [](auto const &qfp) {
        return (qfp.queueFlags & vk::QueueFlagBits::eGraphics) != static_cast<vk::QueueFlags>(0);
      });

  graphicsFamilyIndex = static_cast<uint32_t>(std::distance(queueFamilies.begin(), graphicsQueueFamilyProperties));

  if (graphicsFamilyIndex == queueFamilies.size()) {
    throw std::runtime_error("Could not find a queue that supports graphics");
  }

  presentFamilyIndex =
      physicalDevice.getSurfaceSupportKHR(graphicsFamilyIndex, *surface.Handle())
          ? graphicsFamilyIndex
          : static_cast<uint32_t>(queueFamilies.size());

  if (presentFamilyIndex == queueFamilies.size()) { // Initial graphics queue didn't support presentation, try to find one that supports both
    for (size_t i = 0; i < queueFamilies.size(); i++) {
      if ((queueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics) && physicalDevice.getSurfaceSupportKHR(static_cast<uint32_t>(i), *surface.Handle())) {
        graphicsFamilyIndex = static_cast<uint32_t>(i);
        presentFamilyIndex = graphicsFamilyIndex;
        break;
      }
    }
    if (presentFamilyIndex == queueFamilies.size()) { // If we couldn't find any queues that support both, just take the first queue with presentation support
      for (size_t i = 0; i < queueFamilies.size(); i++) {
        if (physicalDevice.getSurfaceSupportKHR(static_cast<uint32_t>(i), *surface.Handle())) {
          presentFamilyIndex = static_cast<uint32_t>(i);
          break;
        }
      }
    }
  }

  if (presentFamilyIndex == queueFamilies.size()) {
    throw std::runtime_error("Could not find a queue that supports presentation");
  }

  // Create the queue create info objects for vulkan

  std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;

  const float graphicsQueuePriority = 0.5;
  const vk::DeviceQueueCreateInfo graphicsQueueCreateInfo{
      .queueFamilyIndex = graphicsFamilyIndex,
      .queueCount = 1,
      .pQueuePriorities = &graphicsQueuePriority,
  };
  queueCreateInfos.push_back(graphicsQueueCreateInfo);

  if (presentFamilyIndex != graphicsFamilyIndex) { // We have separate queues for presentation and graphics
    const float presentQueuePriority = graphicsQueuePriority;
    const vk::DeviceQueueCreateInfo presentQueueCreateInfo{
        .queueFamilyIndex = presentFamilyIndex,
        .queueCount = 1,
        .pQueuePriorities = &presentQueuePriority,
    };
    queueCreateInfos.push_back(presentQueueCreateInfo);
  }

  // Create the device

  vk::DeviceCreateInfo deviceCreateInfo{
      .pNext = nextDeviceFeatures,
      .queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size()),
      .pQueueCreateInfos = queueCreateInfos.data(),
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
  this->presentQueue = std::make_unique<vk::raii::Queue>(logicalDevice, presentFamilyIndex, 0);
}

} // namespace Vulkan