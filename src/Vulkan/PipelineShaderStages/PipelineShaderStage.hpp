#ifndef PIPELINE_SHADER_STAGE_H
#define PIPELINE_SHADER_STAGE_H

#include "vulkan/vulkan.hpp"
namespace Vulkan {

class ShaderModule;

class PipelineShaderStage {
protected:
  PipelineShaderStage(const vk::ShaderStageFlags shaderStage, const ShaderModule &shaderModule, const std::string &pName);

private:
  const std::string &pName;
  const ShaderModule &shaderModule;
  const vk::ShaderStageFlags shaderStage;
};
} // namespace Vulkan

#endif