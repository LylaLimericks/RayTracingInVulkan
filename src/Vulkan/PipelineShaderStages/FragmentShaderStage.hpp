#ifndef FRAGMENT_SHADER_STAGE_H
#define FRAGMENT_SHADER_STAGE_H

#include "Vulkan/PipelineShaderStages/PipelineShaderStage.hpp"
namespace Vulkan {

class FragmentShaderStage : public PipelineShaderStage {
public:
  FragmentShaderStage(const class ShaderModule &shaderModule, const std::string &pName) : PipelineShaderStage(vk::ShaderStageFlagBits::eFragment, shaderModule, pName){};
};

} // namespace Vulkan

#endif