#ifndef SWAP_CHAIN_H
#define SWAP_CHAIN_H

#include "vulkan/vulkan.hpp"
#include <vulkan/vulkan_raii.hpp>
namespace Vulkan {

class VulkanDevice;
class Window;

class SwapChain {

public:
  SwapChain(const vk::PhysicalDevice &physicalDevice, const VulkanDevice &device, const Window &window, const vk::SurfaceKHR &surface, vk::PresentModeKHR presentMode);
  vk::SurfaceFormatKHR SurfaceFormat() const { return surfaceFormat; }
  const vk::Format *pFormat() const { return &surfaceFormat.format; }

  void RecreateImageViews();
  vk::Image &Image(const uint32_t i) { return swapChainImages[i]; }
  vk::raii::ImageView &ImageView(const uint32_t i) { return swapChainImageViews[i]; }
  const vk::Extent2D Extent() const { return extent; }

  const uint32_t MAX_IMAGE_COUNT = 3u;
  const size_t Size() const { return swapChainImages.size(); }

  vk::ResultValue<uint32_t> AcquireNextFrame(uint64_t timeout, vk::Semaphore semphore);

private:
  const VulkanDevice &device;
  vk::raii::SwapchainKHR swapChain = nullptr;
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