#ifndef COMMAND_BUFFERS_H
#define COMMAND_BUFFERS_H

#include "vulkan/vulkan.hpp"
#include <memory>
#include <vulkan/vulkan_raii.hpp>
namespace Vulkan {
class Device;
class CommandPool;

class CommandBuffer {
public:
  CommandBuffer(const CommandPool &commandPool, const vk::CommandBufferLevel bufferLevel);

  vk::raii::CommandBuffer *Buffer() const { return commandBuffer.get(); }

private:
  const CommandPool &commandPool;
  std::unique_ptr<vk::raii::CommandBuffer> commandBuffer;
};
} // namespace Vulkan

#endif