#ifndef GRAPHICS_PIPELINE_H
#define GRAPHICS_PIPELINE_H

#include <memory>
#include <vector>
#include <vulkan/vulkan_raii.hpp>
namespace Vulkan {

class Device;
class PipelineShaderStage;
class SwapChain;
class PipelineFixedFunctions;
class PipelineLayout;

class GraphicsPipeline {
public:
  GraphicsPipeline(const Device &device, const SwapChain &swapChain, const PipelineFixedFunctions &pipelineStates, const std::vector<PipelineShaderStage> shaderStages, const PipelineLayout &pipelineLayout);
  vk::raii::Pipeline *Handle() const { return graphicsPipeline.get(); }

private:
  const SwapChain &swapChain;
  const Device &device;
  const std::vector<PipelineShaderStage> shaderStages;
  const PipelineFixedFunctions &pipelineStates;
  const PipelineLayout &pipelineLayout;

  std::unique_ptr<vk::raii::Pipeline> graphicsPipeline;
};

} // namespace Vulkan

#endif