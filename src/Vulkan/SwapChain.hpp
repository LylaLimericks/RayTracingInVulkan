#ifndef SWAP_CHAIN_H
#define SWAP_CHAIN_H

#include "vulkan/vulkan.hpp"
#include <vulkan/vulkan_raii.hpp>
namespace Vulkan {

class Device;
class Window;

class SwapChain {

public:
  SwapChain(const Device &device, vk::PresentModeKHR presentMode);
  void RecreateImageViews();

  const uint32_t MAX_IMAGE_COUNT = 3u;

private:
  const Device &device;
  std::unique_ptr<vk::raii::SwapchainKHR> swapChain;
  std::vector<vk::Image> swapChainImages;
  std::vector<vk::raii::ImageView> swapChainImageViews;

  vk::SurfaceFormatKHR surfaceFormat;
  vk::Extent2D extent;

  vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &formats) const;
  vk::PresentModeKHR choosePresentMode(const std::vector<vk::PresentModeKHR> &presentModes, const vk::PresentModeKHR presentMode);
  vk::Extent2D chooseSwapExtent(const Window &window, const vk::SurfaceCapabilitiesKHR &capabilities);
};
} // namespace Vulkan

#endif