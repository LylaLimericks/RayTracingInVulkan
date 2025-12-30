#include "PipelineShaderStage.hpp"
#include "Vulkan/ShaderModule.hpp"
#include "vulkan/vulkan.hpp"
#include <string>

namespace Vulkan {

PipelineShaderStage::PipelineShaderStage(vk::ShaderStageFlagBits shaderStage, const class ShaderModule &shaderModule, const std::string &pName) : shaderStage(shaderStage), shaderModule(shaderModule), pName(pName) {
}

vk::PipelineShaderStageCreateInfo PipelineShaderStage::GetCreateInfo() const {
  return vk::PipelineShaderStageCreateInfo{
      .stage = shaderStage,
      .module = *shaderModule.Handle(),
      .pName = pName.c_str(),
  };
}

} // namespace Vulkan