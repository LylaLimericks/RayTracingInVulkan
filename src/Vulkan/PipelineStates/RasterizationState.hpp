#ifndef RASTERIZATION_STATE_H
#define RASTERIZATION_STATE_H

#include "Vulkan/PipelineStates/PipelineState.hpp"
#include "vulkan/vulkan.hpp"
namespace Vulkan {

using StateCreateInfo = vk::PipelineRasterizationStateCreateInfo;

class RasterizationState : public PipelineState<StateCreateInfo> {
public:
  RasterizationState(StateCreateInfo stateCreateInfo) : PipelineState(stateCreateInfo){};
};
} // namespace Vulkan
#endif