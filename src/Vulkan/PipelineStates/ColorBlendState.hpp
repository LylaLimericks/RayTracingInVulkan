#ifndef COLOR_BLEND_STATE_H
#define COLOR_BLEND_STATE_H

#include "Vulkan/PipelineStates/PipelineState.hpp"
#include "vulkan/vulkan.hpp"
namespace Vulkan {

using StateCreateInfo = vk::PipelineColorBlendStateCreateInfo;

class ColorBlendState : public PipelineState<StateCreateInfo> {
public:
  ColorBlendState(StateCreateInfo stateCreateInfo) : PipelineState(stateCreateInfo){};
};
} // namespace Vulkan

#endif