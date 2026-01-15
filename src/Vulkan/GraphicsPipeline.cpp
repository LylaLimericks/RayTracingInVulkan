#include "GraphicsPipeline.hpp"
#include "Vulkan/FixedFunctions/PipelineFixedFunctions.hpp"
#include "Vulkan/PipelineLayout.hpp"
#include "Vulkan/PipelineShaderStages/PipelineShaderStage.hpp"
#include "Vulkan/SwapChain.hpp"
#include "Vulkan/VulkanDevice.hpp"
#include "vulkan/vulkan.hpp"
#include <vulkan/vulkan_raii.hpp>

namespace Vulkan {
GraphicsPipeline::GraphicsPipeline(
    const VulkanDevice &device,
    const SwapChain &myChain,
    const PipelineFixedFunctions &pipelineStates,
    const std::vector<PipelineShaderStage> shaderStages,
    const PipelineLayout &pipelineLayout)
    : device(device), swapChain(myChain), pipelineStates(pipelineStates), shaderStages(shaderStages), pipelineLayout(pipelineLayout) {

  vk::PipelineRenderingCreateInfo pipelineRenderingCreateInfo{
      .colorAttachmentCount = 1,
      .pColorAttachmentFormats = swapChain.pFormat(),
  };

  vk::PipelineShaderStageCreateInfo shaderStagesCreateInfo[shaderStages.size()];
  for (int i = 0; i < shaderStages.size(); i++) {
    shaderStagesCreateInfo[i] = shaderStages[i].GetCreateInfo();
  }

  std::vector dynamicStates = {
      vk::DynamicState::eViewport,
      vk::DynamicState::eScissor,
  };

  vk::PipelineDynamicStateCreateInfo dynamicState{
      .dynamicStateCount = static_cast<uint32_t>(dynamicStates.size()),
      .pDynamicStates = dynamicStates.data(),
  };

  vk::GraphicsPipelineCreateInfo pipelineInfo{
      .pNext = &pipelineRenderingCreateInfo,
      .stageCount = static_cast<uint32_t>(shaderStages.size()),
      .pStages = shaderStagesCreateInfo,
      .pVertexInputState = pipelineStates.vertexInputState.GetCreateInfo(),
      .pInputAssemblyState = pipelineStates.inputAssemblyState.GetCreateInfo(),
      .pViewportState = pipelineStates.viewportState.GetCreateInfo(),
      .pRasterizationState = pipelineStates.rasterizationState.GetCreateInfo(),
      .pMultisampleState = pipelineStates.multisampleState.GetCreateInfo(),
      .pDynamicState = &dynamicState,
      .layout = pipelineLayout.Handle(),
      .renderPass = nullptr,
  };

  graphicsPipeline = device.CreatePipeline(pipelineInfo);
}
} // namespace Vulkan