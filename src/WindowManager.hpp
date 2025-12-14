#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include <cstdint>

namespace RayTracing {

class WindowManager {
public:
  const char **getRequiredExtensions(uint32_t &extensionCount);
};

} // namespace RayTracing

#endif