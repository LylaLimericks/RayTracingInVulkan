#ifndef DEVICE_H
#define DEVICE_H
#include "Vulkan/Surface.hpp"
#include "vulkan/vulkan.hpp"
#include <memory>
#include <vulkan/vulkan_raii.hpp>
namespace Vulkan {

class Device {
public:
  Device(vk::raii::PhysicalDevice physicalDevice,
         const Surface &surface,
         const std::vector<const char *> enabledExtensions,
         const vk::PhysicalDeviceFeatures &deviceFeatures,
         void *nextDeviceFeatures);

private:
  std::unique_ptr<vk::raii::PhysicalDevice> physicalDevice;
  std::unique_ptr<vk::raii::Device> logicalDevice;
  std::unique_ptr<vk::raii::Queue> graphicsQueue;
  std::unique_ptr<vk::raii::Queue> presentQueue;

  uint32_t graphicsFamilyIndex;
  uint32_t presentFamilyIndex;
};

} // namespace Vulkan
#endif