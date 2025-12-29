#ifndef INPUT_ASSEMBLY_STATE_H
#define INPUT_ASSEMBLY_STATE_H

#include "Vulkan/PipelineStates/PipelineState.hpp"
#include "vulkan/vulkan.hpp"
namespace Vulkan {

using StateCreateInfo = vk::PipelineVertexInputStateCreateInfo;

class InputAssemblyState : public PipelineState<StateCreateInfo> {
public:
  InputAssemblyState(StateCreateInfo createInfo) : PipelineState(createInfo){};
};
} // namespace Vulkan

#endif