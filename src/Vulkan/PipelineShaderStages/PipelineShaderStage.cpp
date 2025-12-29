#include "PipelineShaderStage.hpp"
#include "Vulkan/ShaderModule.hpp"
#include "vulkan/vulkan.hpp"
#include <string>

namespace Vulkan {

PipelineShaderStage::PipelineShaderStage(const vk::ShaderStageFlags shaderStage, const ShaderModule &shaderModule, const std::string &pName) : shaderStage(shaderStage), shaderModule(shaderModule), pName(pName) {
}

} // namespace Vulkan