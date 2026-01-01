#ifndef COMMAND_POOL_H
#define COMMAND_POOL_H

#include "vulkan/vulkan.hpp"
#include <vulkan/vulkan_raii.hpp>
namespace Vulkan {
class Device;

class CommandPool {
public:
  CommandPool(const Device &device, const uint32_t queueIndex) : CommandPool(device, vk::CommandPoolCreateFlagBits::eResetCommandBuffer, queueIndex){};
  CommandPool(const Device &device, const vk::CommandPoolCreateFlagBits createFlagBits, const uint32_t queueIndex);

  const vk::raii::CommandPool *Handle() const { return commandPool.get(); }
  const class Device &Device() const { return device; }

private:
  const class Device &device;
  std::unique_ptr<vk::raii::CommandPool> commandPool;
};
} // namespace Vulkan

#endif