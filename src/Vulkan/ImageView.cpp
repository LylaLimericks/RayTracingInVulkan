#include "ImageView.hpp"

namespace Vulkan {

ImageView::ImageView(const vk::ImageViewType viewType, const vk::Format imageFormat, const vk::ImageAspectFlags aspectFlags, const vk::raii::Image &image) {
  vk::ImageViewCreateInfo imageViewCreateInfo{
      .image = image,
      .viewType = viewType,
      .format = imageFormat,
      .subresourceRange = {aspectFlags, 0, 1, 0, 1},
  };
}
} // namespace Vulkan