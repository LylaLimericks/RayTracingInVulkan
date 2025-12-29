#ifndef IMAGE_VIEW_H
#define IMAGE_VIEW_H

#include "vulkan/vulkan.hpp"
#include <vulkan/vulkan_raii.hpp>
namespace Vulkan {

class Image;

class ImageView {
public:
  ImageView(const vk::ImageViewType viewType, const vk::Format imageFormat, const vk::ImageAspectFlags aspectFlags, const vk::raii::Image &image);

  vk::raii::Image *Image() const { return image.get(); }

private:
  std::unique_ptr<vk::raii::Image> image;
};

} // namespace Vulkan

#endif