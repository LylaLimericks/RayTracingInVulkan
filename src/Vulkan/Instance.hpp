#ifndef INSTANCE_H
#define INSTANCE_H

#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_raii.hpp>
namespace Vulkan {

class ApplicationInfo;
class Window;

class Instance {
public:
  Instance(const Window &window,
           const ApplicationInfo &appInfo,
           const std::vector<const char *> &requiredvalidationLayers,
           bool enableValidationLayers = true);
  // ~Instance();

  const std::vector<vk::raii::PhysicalDevice> PhysicalDevices() const;

  vk::Instance Handle() const { return instance; } // TODO: This is a temp escape hatch for initialization. FIX THIS

  vk::raii::SurfaceKHR CreateSurface(const VkSurfaceKHR &surface) const { return vk::raii::SurfaceKHR(instance, surface); }

private:
  const vk::raii::Context context;
  vk::raii::Instance instance = nullptr;

  const Window &window;
  const ApplicationInfo &appInfo;
  const std::vector<const char *> &validationLayers;

  bool supportsValidationLayers(const std::vector<const char *> &layers) const;
};
} // namespace Vulkan

#endif