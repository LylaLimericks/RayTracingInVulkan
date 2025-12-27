#ifndef APPLICATION_INFO_H
#define APPLICATION_INFO_H

#include "Vulkan/Version.hpp"
#include <string>
#include <vulkan/vulkan_core.h>
namespace Vulkan {

// TODO: Add a generic conversion to vk::ApplicationInfo and remove one off use from Vulkan/Instance.cpp

struct ApplicationInfo {
  const Version AppVersion;
  const Version EngineVersion;

  const std::string EngineName;
};

} // namespace Vulkan

#endif