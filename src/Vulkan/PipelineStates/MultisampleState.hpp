#ifndef MULTISAMPLE_STATE_H
#define MULTISAMPLE_STATE_H

#include "Vulkan/PipelineStates/PipelineState.hpp"
#include "vulkan/vulkan.hpp"
namespace Vulkan {

using StateCreateInfo = vk::PipelineMultisampleStateCreateInfo;

class MultiSampleState : public PipelineState<StateCreateInfo> {
public:
  MultiSampleState(StateCreateInfo stateCreateInfo) : PipelineState(stateCreateInfo){};
};
} // namespace Vulkan
#endif