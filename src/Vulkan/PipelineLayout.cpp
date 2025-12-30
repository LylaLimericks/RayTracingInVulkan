#include "PipelineLayout.hpp"
#include "Vulkan/Device.hpp"

namespace Vulkan {
PipelineLayout::PipelineLayout(const Device &device, const vk::PipelineLayoutCreateInfo &pipelineLayoutInfo) : device(device) {
  pipelineLayout = std::make_unique<vk::raii::PipelineLayout>(*device.Handle(), pipelineLayoutInfo);
}
} // namespace Vulkan