#ifndef VIEWPORT_STATE_H
#define VIEWPORT_STATE_H

#include "FixedFunction.hpp"
#include "vulkan/vulkan.hpp"

namespace Vulkan {

class ViewportState : public FixedFunction<vk::PipelineViewportStateCreateInfo> {
public:
  ViewportState(const vk::PipelineViewportStateCreateInfo &createInfo) : FixedFunction(createInfo){};
};

} // namespace Vulkan

#endif