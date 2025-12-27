#ifndef VERSION_H
#define VERSION_H

#include <cstdint>
namespace Vulkan {
struct Version {
  uint32_t Major;
  uint32_t Minor;
  uint32_t Patch;
};
} // namespace Vulkan

#endif