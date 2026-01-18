#ifndef MULTISAMPLE_STATE_H
#define MULTISAMPLE_STATE_H

#include "FixedFunction.hpp"
#include "vulkan/vulkan.hpp"

namespace Vulkan {

class MultiSampleState : public FixedFunction<vk::PipelineMultisampleStateCreateInfo> {
public:
  MultiSampleState(const vk::PipelineMultisampleStateCreateInfo &stateCreateInfo) : FixedFunction(stateCreateInfo){};
};
} // namespace Vulkan
#endif