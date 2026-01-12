#ifndef PIPELINE_LAYOUT_H
#define PIPELINE_LAYOUT_H

#include <memory>
#include <vulkan/vulkan_raii.hpp>

namespace Vulkan {

class VulkanDevice;

class PipelineLayout {
public:
  PipelineLayout(const VulkanDevice &device, const vk::PipelineLayoutCreateInfo &pipelineLayoutInfo);
  const vk::PipelineLayout Handle() const { return pipelineLayout; }

private:
  vk::raii::PipelineLayout pipelineLayout = nullptr;
  const VulkanDevice &device;
};
} // namespace Vulkan

#endif