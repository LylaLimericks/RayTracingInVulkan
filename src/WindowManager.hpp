#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include <cstdint>

class WindowManager {
public:
  const char **getRequiredExtensions(uint32_t &extensionCount);
};

#endif