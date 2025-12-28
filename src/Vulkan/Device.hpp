#ifndef DEVICE_H
#define DEVICE_H
#include "vulkan/vulkan.hpp"
#include <memory>
#include <vulkan/vulkan_raii.hpp>
namespace Vulkan {

class Surface;

class Device {
public:
  Device(vk::raii::PhysicalDevice physicalDevice,
         const Surface &surface,
         const std::vector<const char *> enabledExtensions,
         const vk::PhysicalDeviceFeatures &deviceFeatures,
         void *nextDeviceFeatures);

  const vk::raii::PhysicalDevice PhysicalDevice() const { return physicalDevice; }
  const Surface &Surface() const { return surface; }
  const vk::raii::Device *Handle() const { return logicalDevice.get(); }

  const uint32_t GraphicsFamilyIndex() const { return graphicsFamilyIndex; }
  const uint32_t PresentFamilyIndex() const { return presentFamilyIndex; }

private:
  const vk::raii::PhysicalDevice physicalDevice;
  const class Surface &surface;
  std::unique_ptr<vk::raii::Device> logicalDevice;
  std::unique_ptr<vk::raii::Queue> graphicsQueue;
  std::unique_ptr<vk::raii::Queue> presentQueue;

  uint32_t graphicsFamilyIndex;
  uint32_t presentFamilyIndex;
};

} // namespace Vulkan
#endif