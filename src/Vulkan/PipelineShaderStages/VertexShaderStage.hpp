#ifndef VERTEX_SHADER_STAGE_H
#define VERTEX_SHADER_STAGE_H

#include "Vulkan//PipelineShaderStages/PipelineShaderStage.hpp"
#include "vulkan/vulkan.hpp"
#include <memory>
namespace Vulkan {

class ShaderModule;

class VertexShaderStage : public PipelineShaderStage {
public:
  VertexShaderStage(std::shared_ptr<ShaderModule> shaderModule, const std::string &pName) : PipelineShaderStage(vk::ShaderStageFlagBits::eVertex, shaderModule, pName){};
};

} // namespace Vulkan
#endif