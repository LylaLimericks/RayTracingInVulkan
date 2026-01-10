#include "CommandBuffer.hpp"
#include "CommandPool.hpp"
#include "Device.hpp"
#include "GraphicsPipeline.hpp"
#include "vulkan/vulkan.hpp"
#include <vulkan/vulkan_core.h>
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

void CommandBuffer::BindPipeline(vk::PipelineBindPoint bindPoint, const GraphicsPipeline &graphicsPipeline) const {
  commandBuffer->bindPipeline(bindPoint, *graphicsPipeline.Handle());
}

void CommandBuffer::TransitionImageLayout(
    vk::Image &image,
    vk::ImageLayout oldLayout,
    vk::ImageLayout newLayout,
    vk::AccessFlags2 srcAccessMask,
    vk::AccessFlags2 dstAccessMask,
    vk::PipelineStageFlags2 srcStageMask,
    vk::PipelineStageFlags2 dstStageMask) {
  vk::ImageMemoryBarrier2 barrier = {
      .srcStageMask = srcStageMask,
      .srcAccessMask = srcAccessMask,
      .dstStageMask = dstStageMask,
      .dstAccessMask = dstAccessMask,
      .oldLayout = oldLayout,
      .newLayout = newLayout,
      .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      .image = image,
      .subresourceRange = {
          .aspectMask = vk::ImageAspectFlagBits::eColor,
          .baseMipLevel = 0,
          .levelCount = 1,
          .baseArrayLayer = 0,
          .layerCount = 1,
      },
  };

  vk::DependencyInfo dependencyInfo = {
      .dependencyFlags = {},
      .imageMemoryBarrierCount = 1,
      .pImageMemoryBarriers = &barrier,
  };

  commandBuffer.get()->pipelineBarrier2(dependencyInfo);
}

void CommandBuffer::Begin(vk::CommandBufferBeginInfo beginInfo) {
  commandBuffer->begin(beginInfo);
}
} // namespace Vulkan