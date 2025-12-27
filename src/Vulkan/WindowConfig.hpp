#ifndef WINDOW_CONFIG_H
#define WINDOW_CONFIG_H

#include <cstdint>
#include <string>

namespace Vulkan {
class WindowConfig {
public:
  std::string Title;
  uint32_t Width;
  uint32_t Height;
};
} // namespace Vulkan

#endif