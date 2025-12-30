#ifndef PIPELINE_SHADER_STAGE_H
#define PIPELINE_SHADER_STAGE_H

#include "vulkan/vulkan.hpp"
namespace Vulkan {

class ShaderModule;

class PipelineShaderStage {
public:
  vk::PipelineShaderStageCreateInfo GetCreateInfo() const;

protected:
  PipelineShaderStage(vk::ShaderStageFlagBits shaderStage, const ShaderModule &shaderModule, const std::string &pName);
  const class ShaderModule &ShaderModule() const { return shaderModule; }

private:
  const std::string &pName;
  const class ShaderModule &shaderModule;
  const vk::ShaderStageFlagBits shaderStage;
};
} // namespace Vulkan

#endif