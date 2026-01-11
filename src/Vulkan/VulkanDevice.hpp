#ifndef DEVICE_H
#define DEVICE_H
#include "vulkan/vulkan.hpp"
#include <memory>
#include <vulkan/vulkan_raii.hpp>
namespace Vulkan {

class VulkanDevice {
public:
  VulkanDevice(vk::raii::PhysicalDevice physicalDevice,
               const vk::SurfaceKHR &surface,
               const std::vector<const char *> &enabledExtensions,
               const vk::PhysicalDeviceFeatures &deviceFeatures,
               void *nextDeviceFeatures);
  ~VulkanDevice();

  const uint32_t GraphicsFamilyIndex() const { return graphicsFamilyIndex; }
  const uint32_t PresentFamilyIndex() const { return presentFamilyIndex; }
  vk::raii::CommandPool CreateCommandPool(const vk::CommandPoolCreateFlagBits createFlagBits, const uint32_t queueIndex);
  std::vector<vk::raii::CommandBuffer> CreateCommandBuffers(const vk::CommandBufferLevel commandBufferLevel, const uint32_t &bufferCount);
  std::vector<vk::raii::CommandBuffer> CreateCommandBuffers(const vk::CommandBufferLevel commandBufferLevel, const vk::CommandPool &commandPool, const uint32_t &bufferCount);
  const vk::Device LogicalDevice() const { return logicalDevice; }
  explicit operator const vk::Device() const { return logicalDevice; }

  std::unique_ptr<vk::raii::SwapchainKHR> CreateSwapChain(const vk::SwapchainCreateInfoKHR createInfo) const;
  vk::raii::ImageView CreateImageView(const vk::ImageViewCreateInfo imageViewCreateInfo) const;

private:
  const std::vector<const char *> &enabledExtensions;
  const vk::raii::PhysicalDevice physicalDevice;
  vk::raii::Device logicalDevice = nullptr;
  vk::raii::Queue graphicsQueue = nullptr;
  vk::raii::Queue presentQueue = nullptr;
  vk::raii::CommandPool commandPool = nullptr;

  uint32_t graphicsFamilyIndex;
  uint32_t presentFamilyIndex;
};

} // namespace Vulkan
#endif