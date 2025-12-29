#ifndef VIEWPORT_STATE_H
#define VIEWPORT_STATE_H

#include "Vulkan/PipelineStates/PipelineState.hpp"
#include "vulkan/vulkan.hpp"
namespace Vulkan {

using StateCreateInfo = vk::PipelineViewportStateCreateInfo;

class ViewportState : public PipelineState<StateCreateInfo> {
public:
  ViewportState(StateCreateInfo createInfo) : PipelineState(createInfo){};
};

} // namespace Vulkan

#endif