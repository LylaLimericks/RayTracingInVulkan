#ifndef PIPELINE_SHADER_STATE_H
#define PIPELINE_SHADER_STATE_H

namespace Vulkan {
// Abstract Base Class for ShaderStates
template <class StateCreateInfo>
class FixedFunction {
public:
  // virtual void SetCreateInfo(StateCreateInfo stateCreateInfo);
  virtual const StateCreateInfo *GetCreateInfo() const { return &createInfo; }

protected:
  FixedFunction(const StateCreateInfo &stateCreateInfo) : createInfo(stateCreateInfo){};
  const StateCreateInfo createInfo;
};
} // namespace Vulkan

#endif