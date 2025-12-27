#include "Application.hpp"
#include "Window.hpp"
#include <vulkan/vulkan_core.h>

namespace Vulkan {

Application::Application(const ApplicationInfo &appInfo,
                         const WindowConfig &windowConfig,
                         VkPresentModeKHR presentMode,
                         bool enableValidationLayers) {
  const auto validationLayers =
      enableValidationLayers
          ? std::vector<const char *>{"VK_LAYER_KHRONOS_validation"}
          : std::vector<const char *>();

  window = std::make_unique<Window>(windowConfig);
}

} // namespace Vulkan