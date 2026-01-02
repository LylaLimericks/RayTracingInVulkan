#include "CommandBuffer.hpp"
#include "CommandPool.hpp"
#include "Vulkan/Device.hpp"
#include "vulkan/vulkan.hpp"
#include <vulkan/vulkan_raii.hpp>

namespace Vulkan {
CommandBuffer::CommandBuffer(const CommandPool &commandPool, const vk::CommandBufferLevel bufferLevel) : commandPool(commandPool) {
  vk::CommandBufferAllocateInfo allocInfo{
      .commandPool = *commandPool.Handle(),
      .level = bufferLevel,
      .commandBufferCount = static_cast<uint32_t>(1),
  };
  auto temp = std::move(vk::raii::CommandBuffers(*commandPool.Device().Handle(), allocInfo).front());
  commandBuffer.reset(&temp);
}
} // namespace Vulkan