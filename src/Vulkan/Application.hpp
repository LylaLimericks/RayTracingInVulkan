#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>
#include <vector>
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_raii.hpp>
namespace Vulkan {

class Window;
class Instance;
class VulkanDevice;
class Surface;
class GraphicsPipeline;
class SwapChain;
class PipelineLayout;
class PipelineFixedFunctions;
class PipelineShaderStage;
class ApplicationInfo;
class WindowConfig;
class ShaderModule;

class Application {
public:
  Application(const ApplicationInfo &appInfo, const WindowConfig &windowConfig,
              vk::PresentModeKHR presentMode, bool enableValidationLayers);
  ~Application();

  void setPhysicalDevice(VulkanDevice device);
  void initializeGraphicsPipeline();
  void createSwapChain();

  void TransitionImageLayout(
      const vk::Image &image,
      const vk::ImageLayout &oldLayout,
      const vk::ImageLayout &newLayout,
      const vk::AccessFlags2 &srcAccessMask,
      const vk::AccessFlags2 &dstAccessMask,
      const vk::PipelineStageFlags2 &srcStageMask,
      const vk::PipelineStageFlags2 &dstStageMask,
      const vk::ImageAspectFlags &imageAspectFlags,
      const uint32_t &currentFrame) const;

protected:
  const std::string ShaderModules[1]{
      "shaders/slang.spv",
  };

  const vk::PresentModeKHR presentMode;

  vk::raii::PhysicalDevice physicalDevice = nullptr;
  std::unique_ptr<Instance> instance;
  std::unique_ptr<Window> window;
  std::unique_ptr<VulkanDevice> device;
  std::unique_ptr<Surface> surface;
  std::unique_ptr<GraphicsPipeline> graphicsPipeline;
  std::unique_ptr<SwapChain> swapChain;
  std::unique_ptr<PipelineLayout> pipelineLayout;
  std::shared_ptr<ShaderModule> shaderModule;
  std::vector<PipelineShaderStage> shaderStages;
  std::vector<vk::raii::CommandBuffer> commandBuffers;

  // TODO: Determine if we should allow for custom physical device selection.
  // This is initially just to select the default for the getting started

  size_t frameCount = 0;

  void pickDefaultPhysicalDevice();
  PipelineFixedFunctions GetFixedFunctions();
  void CreatePipelineLayout();
};
} // namespace Vulkan

#endif