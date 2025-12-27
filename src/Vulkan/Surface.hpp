#ifndef SURFACE_H
#define SURFACE_H

#include "Vulkan/Instance.hpp"
#include "Vulkan/Window.hpp"
#include <vulkan/vulkan_raii.hpp>
namespace Vulkan {

class Surface {
public:
  Surface(const Instance &instance, const Window &window);

private:
  std::unique_ptr<vk::raii::SurfaceKHR> surface;
};

} // namespace Vulkan

#endif