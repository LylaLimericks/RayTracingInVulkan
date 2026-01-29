#ifndef PIPELINE_SHADER_STAGE_H
#define PIPELINE_SHADER_STAGE_H

#include "vulkan/vulkan.hpp"
#include <memory>
namespace Vulkan {

class ShaderModule;

class PipelineShaderStage {
public:
  vk::PipelineShaderStageCreateInfo GetCreateInfo() const;
  ~PipelineShaderStage();

protected:
  PipelineShaderStage(vk::ShaderStageFlagBits shaderStage, std::shared_ptr<ShaderModule> shaderModule, const std::string &pName) : shaderStage(shaderStage), shaderModule(move(shaderModule)), pName(pName){};

private:
  const std::string &pName;
  std::shared_ptr<class ShaderModule> shaderModule;
  const vk::ShaderStageFlagBits shaderStage;
};
} // namespace Vulkan

#endif