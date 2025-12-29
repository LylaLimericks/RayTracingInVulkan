#ifndef PIPELINE_SHADER_STATE_H
#define PIPELINE_SHADER_STATE_H

namespace Vulkan {
// Abstract Base Class for ShaderStates
template <class StateCreateInfo>
class PipelineState {
public:
  virtual void setCreateInfo(StateCreateInfo stateCreateInfo);
  virtual StateCreateInfo getCreateInfo();

protected:
  PipelineState(StateCreateInfo stateCreateInfo) : createInfo(stateCreateInfo){};
  const StateCreateInfo createInfo;
};
} // namespace Vulkan

#endif