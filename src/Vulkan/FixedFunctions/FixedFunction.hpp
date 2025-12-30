#ifndef PIPELINE_SHADER_STATE_H
#define PIPELINE_SHADER_STATE_H

namespace Vulkan {
// Abstract Base Class for ShaderStates
template <class StateCreateInfo>
class FixedFunction {
public:
  virtual void SetCreateInfo(StateCreateInfo stateCreateInfo);
  virtual StateCreateInfo *GetCreateInfo() const { return &createInfo; }

protected:
  FixedFunction(StateCreateInfo stateCreateInfo) : createInfo(stateCreateInfo){};
  StateCreateInfo createInfo;
};
} // namespace Vulkan

#endif