#ifndef APPLICATION_H
#define APPLICATION_H

#include "Vulkan/ApplicationInfo.hpp"
#include "Vulkan/Device.hpp"
#include "Vulkan/Instance.hpp"
#include "Vulkan/WindowConfig.hpp"
#include <vulkan/vulkan_core.h>
namespace Vulkan {

class Window;

class Application {
public:
  Application(const ApplicationInfo &appInfo, const WindowConfig &windowConfig,
              VkPresentModeKHR presentMode, bool enableValidationLayers);

  void setDevice(const Vulkan::Device &device);

private:
  std::unique_ptr<Instance> instance;
  std::unique_ptr<Window> window;
};
} // namespace Vulkan

#endif