#ifndef COMMAND_BUFFERS_H
#define COMMAND_BUFFERS_H

#include "Vulkan/GraphicsPipeline.hpp"
#include "vulkan/vulkan.hpp"
#include <memory>
#include <vulkan/vulkan_raii.hpp>
namespace Vulkan {
class Device;
class CommandPool;

class CommandBuffer {
public:
  CommandBuffer(const CommandPool &commandPool, const vk::CommandBufferLevel bufferLevel);

  const vk::raii::CommandBuffer *Buffer() const { return commandBuffer.get(); }

  void BindPipeline(vk::PipelineBindPoint bindPoint, const GraphicsPipeline &graphicsPipeline) const;

  void Begin(vk::CommandBufferBeginInfo beginInfo);

  void TransitionImageLayout(
      vk::Image &image,
      vk::ImageLayout oldLayout,
      vk::ImageLayout newLayout,
      vk::AccessFlags2 srcAccessMask,
      vk::AccessFlags2 dstAccessMask,
      vk::PipelineStageFlags2 srcStageMask,
      vk::PipelineStageFlags2 dstStageMask);

private:
  const CommandPool &commandPool;
  std::unique_ptr<vk::raii::CommandBuffer> commandBuffer;
};
} // namespace Vulkan

#endif