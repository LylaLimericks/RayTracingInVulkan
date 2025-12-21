#ifndef DEVICE_H
#define DEVICE_H
#include "vulkan/vulkan.hpp"
#include <memory>
#include <vulkan/vulkan_raii.hpp>
namespace Vulkan {

class Device {
public:
  Device(vk::raii::PhysicalDevice physicalDevice,
         const std::vector<const char *> enabledExtensions);

private:
  std::unique_ptr<vk::raii::PhysicalDevice> physicalDevice;
  std::unique_ptr<vk::raii::Device> logicalDevice;
  std::unique_ptr<vk::raii::Queue> graphicsQueue;

  uint32_t findQueueFamily(const vk::QueueFlags flags);

  uint32_t graphicsFamilyIndex;
};

} // namespace Vulkan
#endif