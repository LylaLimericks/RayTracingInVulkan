#ifndef VERTEX_INPUT_STATE
#define VERTEX_INPUT_STATE

#include "Vulkan/PipelineStates/PipelineState.hpp"
#include "vulkan/vulkan.hpp"
namespace Vulkan {

using StateCreateInfo = vk::PipelineVertexInputStateCreateInfo;

class VertexInputState : public PipelineState<StateCreateInfo> {
public:
  VertexInputState(StateCreateInfo stateCreateInfo) : PipelineState(stateCreateInfo){};
};
} // namespace Vulkan

#endif