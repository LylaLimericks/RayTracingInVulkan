#ifndef SURFACE_H
#define SURFACE_H

#include <vulkan/vulkan_raii.hpp>
namespace Vulkan {

class Window;
class Instance;

class Surface {
public:
  Surface(const Instance &instance, const Window &window);

  const vk::raii::SurfaceKHR *Handle() const { return surface.get(); }

  const class Window &Window() const { return window; }
  const class Instance &Instance() const { return instance; }

private:
  const class Window &window;
  const class Instance &instance;
  std::unique_ptr<vk::raii::SurfaceKHR> surface;
};

} // namespace Vulkan

#endif