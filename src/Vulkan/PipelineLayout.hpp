#ifndef PIPELINE_LAYOUT_H
#define PIPELINE_LAYOUT_H

#include <memory>
#include <vulkan/vulkan_raii.hpp>

namespace Vulkan {

class Device;

class PipelineLayout {
public:
  PipelineLayout(const Device &device, const vk::PipelineLayoutCreateInfo &pipelineLayoutInfo);
  vk::raii::PipelineLayout *Handle() const { return pipelineLayout.get(); }

private:
  std::unique_ptr<vk::raii::PipelineLayout> pipelineLayout;
  const Device &device;
};
} // namespace Vulkan

#endif