#include "VulkanDevice.hpp"
#include "vulkan/vulkan.hpp"
#include <memory>
#include <stdexcept>
#include <vulkan/vulkan_raii.hpp>

namespace Vulkan {

VulkanDevice::VulkanDevice(vk::raii::PhysicalDevice physicalDevice,
                           const vk::SurfaceKHR &surface,
                           const std::vector<const char *> &enabledExtensions,
                           const vk::PhysicalDeviceFeatures &deviceFeatures,
                           void *nextDeviceFeatures)
    : physicalDevice(physicalDevice), enabledExtensions(enabledExtensions) {

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
      physicalDevice.getSurfaceSupportKHR(graphicsFamilyIndex, surface)
          ? graphicsFamilyIndex
          : static_cast<uint32_t>(queueFamilies.size());

  if (presentFamilyIndex == queueFamilies.size()) { // Initial graphics queue didn't support presentation, try to find one that supports both
    for (size_t i = 0; i < queueFamilies.size(); i++) {
      if ((queueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics) && physicalDevice.getSurfaceSupportKHR(static_cast<uint32_t>(i), surface)) {
        graphicsFamilyIndex = static_cast<uint32_t>(i);
        presentFamilyIndex = graphicsFamilyIndex;
        break;
      }
    }
    if (presentFamilyIndex == queueFamilies.size()) { // If we couldn't find any queues that support both, just take the first queue with presentation support
      for (size_t i = 0; i < queueFamilies.size(); i++) {
        if (physicalDevice.getSurfaceSupportKHR(static_cast<uint32_t>(i), surface)) {
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

  logicalDevice = vk::raii::Device(physicalDevice, deviceCreateInfo);
  graphicsQueue = vk::raii::Queue(vk::raii::Queue(logicalDevice, graphicsFamilyIndex, 0));
  presentQueue = vk::raii::Queue(vk::raii::Queue(logicalDevice, presentFamilyIndex, 0));
  commandPool = CreateCommandPool(vk::CommandPoolCreateFlagBits::eResetCommandBuffer, graphicsFamilyIndex);
}

vk::raii::CommandPool VulkanDevice::CreateCommandPool(const vk::CommandPoolCreateFlagBits createFlagBits, const uint32_t queueIndex) {
  vk::CommandPoolCreateInfo poolInfo{
      .flags = createFlagBits,
      .queueFamilyIndex = queueIndex,
  };

  return vk::raii::CommandPool(logicalDevice, poolInfo);
}

std::vector<vk::raii::CommandBuffer> VulkanDevice::CreateCommandBuffers(const vk::CommandBufferLevel bufferLevel, const vk::CommandPool &commandPool, const uint32_t &bufferCount) {
  vk::CommandBufferAllocateInfo allocInfo{
      .commandPool = commandPool,
      .level = bufferLevel,
      .commandBufferCount = bufferCount,
  };

  return vk::raii::CommandBuffers(logicalDevice, allocInfo);
}

std::unique_ptr<vk::raii::SwapchainKHR> VulkanDevice::CreateSwapChain(const vk::SwapchainCreateInfoKHR createInfo) const {
  return std::make_unique<vk::raii::SwapchainKHR>(logicalDevice, createInfo);
}

vk::raii::ImageView VulkanDevice::CreateImageView(const vk::ImageViewCreateInfo imageViewCreateInfo) const {
  return vk::raii::ImageView(logicalDevice, imageViewCreateInfo);
}

} // namespace Vulkan