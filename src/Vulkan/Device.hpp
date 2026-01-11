#ifndef DEVICE_H
#define DEVICE_H
#include "Vulkan/CommandBuffer.hpp"
#include "vulkan/vulkan.hpp"
#include <memory>
#include <vulkan/vulkan_raii.hpp>
namespace Vulkan {

class Device {
public:
  Device(vk::raii::PhysicalDevice physicalDevice,
         const vk::SurfaceKHR &surface,
         const std::vector<const char *> enabledExtensions,
         const vk::PhysicalDeviceFeatures &deviceFeatures,
         void *nextDeviceFeatures);
  ~Device();

  const uint32_t GraphicsFamilyIndex() const { return graphicsFamilyIndex; }
  const uint32_t PresentFamilyIndex() const { return presentFamilyIndex; }
  std::unique_ptr<vk::raii::CommandPool> CreateCommandPool(const vk::CommandPoolCreateFlagBits createFlagBits, const uint32_t queueIndex);
  std::vector<vk::raii::CommandBuffer> CreateCommandBuffers(const vk::CommandBufferLevel commandBufferLevel, const vk::CommandPool &commandPool, const uint32_t &bufferCount);

private:
  const vk::raii::PhysicalDevice physicalDevice;
  std::unique_ptr<vk::raii::Device> logicalDevice;
  std::unique_ptr<vk::raii::Queue> graphicsQueue;
  std::unique_ptr<vk::raii::Queue> presentQueue;
  std::unique_ptr<vk::raii::CommandPool> commandPool;

  uint32_t graphicsFamilyIndex;
  uint32_t presentFamilyIndex;
};

} // namespace Vulkan
#endif