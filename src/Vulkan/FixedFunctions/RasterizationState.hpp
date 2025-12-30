#ifndef RASTERIZATION_STATE_H
#define RASTERIZATION_STATE_H

#include "FixedFunction.hpp"
#include "vulkan/vulkan.hpp"

namespace Vulkan {

class RasterizationState : public FixedFunction<vk::PipelineRasterizationStateCreateInfo> {
public:
  RasterizationState(vk::PipelineRasterizationStateCreateInfo stateCreateInfo) : FixedFunction(stateCreateInfo){};
};
} // namespace Vulkan
#endif