#ifndef GRAPHICS_PIPELINE_H
#define GRAPHICS_PIPELINE_H

#include <vector>
#include <vulkan/vulkan_raii.hpp>
namespace Vulkan {

class VulkanDevice;
class PipelineShaderStage;
class SwapChain;
class PipelineFixedFunctions;
class PipelineLayout;

class GraphicsPipeline {
public:
  GraphicsPipeline(const VulkanDevice &device, const SwapChain &swapChain, const PipelineFixedFunctions &pipelineStates, const std::vector<PipelineShaderStage> shaderStages, const PipelineLayout &pipelineLayout);
  const vk::Pipeline Handle() const { return graphicsPipeline; }

private:
  const SwapChain &swapChain;
  const VulkanDevice &device;
  const std::vector<PipelineShaderStage> shaderStages;
  const PipelineFixedFunctions &pipelineStates;
  const PipelineLayout &pipelineLayout;

  vk::raii::Pipeline graphicsPipeline = nullptr;
};

} // namespace Vulkan

#endif