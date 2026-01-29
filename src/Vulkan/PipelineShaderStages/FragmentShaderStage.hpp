#ifndef FRAGMENT_SHADER_STAGE_H
#define FRAGMENT_SHADER_STAGE_H

#include "Vulkan/PipelineShaderStages/PipelineShaderStage.hpp"
#include <memory>
namespace Vulkan {

class FragmentShaderStage : public PipelineShaderStage {
public:
  FragmentShaderStage(std::shared_ptr<ShaderModule> shaderModule, const std::string &pName) : PipelineShaderStage(vk::ShaderStageFlagBits::eFragment, shaderModule, pName){};
};

} // namespace Vulkan

#endif