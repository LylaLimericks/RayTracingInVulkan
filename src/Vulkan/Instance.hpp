#ifndef INSTANCE_H
#define INSTANCE_H

#include "Vulkan/ApplicationInfo.hpp"
#include <memory>
#include <vulkan/vulkan_raii.hpp>
namespace Vulkan {

class Window;

class Instance {
public:
  Instance(const Window &window,
           const ApplicationInfo &appInfo,
           const std::vector<const char *> &requiredvalidationLayers,
           bool enableValidationLayers = true);
  ~Instance();

  const std::vector<vk::raii::PhysicalDevice> PhysicalDevices() const;

  vk::Instance Handle() const { return instance; } // TODO: This is a temp escape hatch for initialization. FIX THIS

private:
  vk::raii::Context context;
  vk::raii::Instance instance = nullptr;

  const Window &window;
  const ApplicationInfo &appInfo;
  const std::vector<const char *> validationLayers;

  bool supportsValidationLayers(const std::vector<const char *> &layers) const;
};
} // namespace Vulkan

#endif