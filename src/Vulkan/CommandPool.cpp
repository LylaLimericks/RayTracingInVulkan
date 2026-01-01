#include "CommandPool.hpp"
#include "Vulkan/Device.hpp"
#include "vulkan/vulkan.hpp"
#include <vulkan/vulkan_raii.hpp>

namespace Vulkan {
CommandPool::CommandPool(const class Device &device, const vk::CommandPoolCreateFlagBits createFlagBits, const uint32_t queueIndex) : device(device) {
  vk::CommandPoolCreateInfo poolInfo{
      .flags = createFlagBits,
      .queueFamilyIndex = queueIndex};

  commandPool = std::make_unique<vk::raii::CommandPool>(*device.Handle(), poolInfo);
}
} // namespace Vulkan