#ifndef APPLICATION_H
#define APPLICATION_H

#include "Vulkan/ApplicationInfo.hpp"
#include "Vulkan/Device.hpp"
#include "Vulkan/Instance.hpp"
#include "Vulkan/WindowConfig.hpp"
#include <memory>
#include <vulkan/vulkan_core.h>
namespace Vulkan {

class Window;

class Application {
public:
  Application(const ApplicationInfo &appInfo, const WindowConfig &windowConfig,
              VkPresentModeKHR presentMode, bool enableValidationLayers);

  void setPhysicalDevice(Device device);

private:
  std::unique_ptr<Instance> instance;
  std::unique_ptr<Window> window;
  std::unique_ptr<Device> device;
  std::unique_ptr<Surface> surface;
  // TODO: Determine if we should allow for custom physical device selection.
  // This is initially just to select the default for the getting started
  void pickDefaultPhysicalDevice();
};
} // namespace Vulkan

#endif