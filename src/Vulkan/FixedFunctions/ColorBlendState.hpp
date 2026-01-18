#ifndef COLOR_BLEND_STATE_H
#define COLOR_BLEND_STATE_H

#include "FixedFunction.hpp"
#include "vulkan/vulkan.hpp"

namespace Vulkan {

class ColorBlendState : public FixedFunction<vk::PipelineColorBlendStateCreateInfo> {
public:
  ColorBlendState(const vk::PipelineColorBlendStateCreateInfo &stateCreateInfo) : FixedFunction(stateCreateInfo){};
};
} // namespace Vulkan

#endif