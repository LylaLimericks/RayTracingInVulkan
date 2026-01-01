#ifndef COMMAND_BUFFER_H
#define COMMAND_BUFFER_H

#include "vulkan/vulkan.hpp"
#include <vulkan/vulkan_raii.hpp>
namespace Vulkan {
class Device;
class CommandPool;

class CommandBuffers {
public:
  CommandBuffers(const CommandPool &commandPool, const vk::CommandBufferLevel bufferLevel);

  uint32_t size() const { return static_cast<uint32_t>(commandBuffers.size()); }
  vk::raii::CommandBuffer &operator[](const size_t i) { return commandBuffers[i]; }

private:
  const CommandPool &commandPool;
  std::vector<vk::raii::CommandBuffer> commandBuffers;
};
} // namespace Vulkan

#endif