#include "SwapChain.hpp"
#include "Vulkan/Device.hpp"
#include "Vulkan/Surface.hpp"
#include "Vulkan/Window.hpp"
#include "vulkan/vulkan.hpp"
#include <memory>
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_raii.hpp>

namespace Vulkan {

SwapChain::SwapChain(const Device &device, vk::PresentModeKHR presentMode) : device(device) {
  const auto physicalDevice = device.PhysicalDevice();
  const Surface &surface = device.Surface();
  const Window &window = surface.Window();
  const auto surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR(*surface.Handle());

  surfaceFormat = chooseSwapSurfaceFormat(physicalDevice.getSurfaceFormatsKHR());
  extent = chooseSwapExtent(window, surfaceCapabilities);

  auto minImageCount = std::max(MAX_IMAGE_COUNT, surfaceCapabilities.minImageCount);
  minImageCount =
      (surfaceCapabilities.maxImageCount > 0 && minImageCount > surfaceCapabilities.maxImageCount)
          ? surfaceCapabilities.maxImageCount
          : minImageCount;

  vk::SwapchainCreateInfoKHR swapChainCreateInfo{
      .flags = vk::SwapchainCreateFlagsKHR(),
      .surface = *surface.Handle(),
      .minImageCount = minImageCount,
      .imageFormat = surfaceFormat.format,
      .imageColorSpace = surfaceFormat.colorSpace,
      .imageExtent = extent,
      .imageArrayLayers = 1,
      .imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
      .imageSharingMode = vk::SharingMode::eExclusive,
      .preTransform = surfaceCapabilities.currentTransform,
      .compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque,
      .presentMode = choosePresentMode(physicalDevice.getSurfacePresentModesKHR(*surface.Handle()), presentMode),
      .clipped = true,
      .oldSwapchain = VK_NULL_HANDLE,
  };

  if (device.GraphicsFamilyIndex() != device.PresentFamilyIndex()) { // If we need to have separate graphics and present families
    uint32_t queueFamilyIndices[] = {device.GraphicsFamilyIndex(), device.PresentFamilyIndex()};
    swapChainCreateInfo.imageSharingMode = vk::SharingMode::eConcurrent;
    swapChainCreateInfo.queueFamilyIndexCount = 2;
    swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
  }

  swapChain = std::make_unique<vk::raii::SwapchainKHR>(*device.Handle(), swapChainCreateInfo);
  swapChainImages = swapChain->getImages();
  RecreateImageViews();
}

void SwapChain::RecreateImageViews() {
  swapChainImageViews.clear();

  vk::ImageViewCreateInfo imageViewCreateInfo{
      .viewType = vk::ImageViewType::e2D,
      .format = surfaceFormat.format,
      .subresourceRange = {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1},
  };

  for (const auto &image : swapChainImages) {
    imageViewCreateInfo.image = image;
    swapChainImageViews.emplace_back(*device.Handle(), imageViewCreateInfo);
  }
}

vk::ResultValue<uint32_t> SwapChain::AcquireNextFrame(uint64_t timeout, vk::Semaphore semphore) {
  return swapChain->acquireNextImage(timeout, semphore, nullptr);
}

vk::SurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &availableFormats) const {

  for (const auto &availableFormat : availableFormats) {
    if (availableFormat.format == vk::Format::eB8G8R8A8Srgb && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
      return availableFormat;
    }
  }

  return availableFormats[0];
}

vk::PresentModeKHR SwapChain::choosePresentMode(const std::vector<vk::PresentModeKHR> &presentModes, vk::PresentModeKHR presentMode) {
  if (std::find(presentModes.begin(), presentModes.end(), presentMode) != presentModes.end()) {
    return presentMode;
  }

  return vk::PresentModeKHR::eFifo;
}

vk::Extent2D SwapChain::chooseSwapExtent(const Window &window, const vk::SurfaceCapabilitiesKHR &capabilities) {
  if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
    return capabilities.currentExtent;
  }

  const auto windowSize = window.FramebufferSize();

  return {
      std::clamp<uint32_t>(windowSize.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
      std::clamp<uint32_t>(windowSize.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height),
  };
}

} // namespace Vulkan