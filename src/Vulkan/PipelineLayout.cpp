#include "PipelineLayout.hpp"
#include "Vulkan/VulkanDevice.hpp"

namespace Vulkan {
PipelineLayout::PipelineLayout(const VulkanDevice &device, const vk::PipelineLayoutCreateInfo &pipelineLayoutInfo) : device(device) {
  pipelineLayout = device.CreatePipelineLayout(pipelineLayoutInfo);
}
} // namespace Vulkan