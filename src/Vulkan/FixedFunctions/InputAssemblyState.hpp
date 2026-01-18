#ifndef INPUT_ASSEMBLY_STATE_H
#define INPUT_ASSEMBLY_STATE_H

#include "FixedFunction.hpp"
#include "vulkan/vulkan.hpp"

namespace Vulkan {

class InputAssemblyState : public FixedFunction<vk::PipelineInputAssemblyStateCreateInfo> {
public:
  InputAssemblyState(const vk::PipelineInputAssemblyStateCreateInfo &createInfo) : FixedFunction(createInfo){};
};
} // namespace Vulkan

#endif