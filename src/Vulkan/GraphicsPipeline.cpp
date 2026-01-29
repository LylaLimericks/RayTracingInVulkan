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

  vk::StructureChain<vk::GraphicsPipelineCreateInfo, vk::PipelineRenderingCreateInfo> pipelineInfo{
      {
          .stageCount = static_cast<uint32_t>(shaderStages.size()),
          .pStages = shaderStagesCreateInfo,
          .pVertexInputState = pipelineStates.vertexInputState.GetCreateInfo(),
          .pInputAssemblyState = pipelineStates.inputAssemblyState.GetCreateInfo(),
          .pViewportState = pipelineStates.viewportState.GetCreateInfo(),
          .pRasterizationState = pipelineStates.rasterizationState.GetCreateInfo(),
          .pMultisampleState = pipelineStates.multisampleState.GetCreateInfo(),
          .pColorBlendState = pipelineStates.colorBlendState.GetCreateInfo(),
          .pDynamicState = &dynamicState,
          .layout = pipelineLayout.Handle(),
          .renderPass = nullptr,
      },
      {
          .colorAttachmentCount = 1,
          .pColorAttachmentFormats = swapChain.pFormat(),
      }};

  graphicsPipeline = device.CreatePipeline(pipelineInfo.get<vk::GraphicsPipelineCreateInfo>());
}
} // namespace Vulkan