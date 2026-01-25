#ifndef VERTEX_INPUT_STATE
#define VERTEX_INPUT_STATE

#include "FixedFunction.hpp"
#include "vulkan/vulkan.hpp"

namespace Vulkan {

class VertexInputState : public FixedFunction<vk::PipelineVertexInputStateCreateInfo> {
public:
  VertexInputState(const vk::PipelineVertexInputStateCreateInfo &stateCreateInfo) : FixedFunction(stateCreateInfo){};

  using FixedFunction<vk::PipelineVertexInputStateCreateInfo>::GetCreateInfo;
};
} // namespace Vulkan

#endif