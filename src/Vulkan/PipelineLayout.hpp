#ifndef PIPELINE_LAYOUT_H
#define PIPELINE_LAYOUT_H

#include <memory>
#include <vulkan/vulkan_raii.hpp>

namespace Vulkan {

class PipelineLayout {
public:
  PipelineLayout();
  vk::raii::PipelineLayout *Handle() const { return pipelineLayout.get(); }

private:
  std::unique_ptr<vk::raii::PipelineLayout> pipelineLayout;
};
} // namespace Vulkan

#endif