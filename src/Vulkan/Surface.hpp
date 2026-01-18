#ifndef SURFACE_H
#define SURFACE_H

#include <vulkan/vulkan_raii.hpp>
namespace Vulkan {

class Window;
class Instance;

class Surface {
public:
  Surface(const Instance &instance, const Window &window);
  ~Surface();

  const vk::raii::SurfaceKHR *Handle() const { return &surface; }

  const class Window &Window() const { return window; }
  const class Instance &Instance() const { return instance; }

private:
  const class Window &window;
  const class Instance &instance;
  vk::raii::SurfaceKHR surface = nullptr;
};

} // namespace Vulkan

#endif