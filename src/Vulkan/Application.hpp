#ifndef APPLICATION_H
#define APPLICATION_H

#include "Vulkan/CommandBuffer.hpp"
#include "Vulkan/PipelineShaderStages/PipelineShaderStage.hpp"
#include <memory>
#include <vector>
#include <vulkan/vulkan_core.h>
namespace Vulkan {

class Window;
class Instance;
class Device;
class CommandBuffer;
class CommandPool;
class Surface;
class GraphicsPipeline;
class SwapChain;
class PipelineLayout;
class PipelineFixedFunctions;
class ApplicationInfo;
class WindowConfig;
class ShaderModule;

class Application {
public:
  Application(const ApplicationInfo &appInfo, const WindowConfig &windowConfig,
              vk::PresentModeKHR presentMode, bool enableValidationLayers);

  void setPhysicalDevice(Device device);
  void initializeGraphicsPipeline();
  void createSwapChain();

protected:
  const std::string ShaderModules[1]{
      "shaders/slang.spv",
  };

  const vk::PresentModeKHR presentMode;

  std::unique_ptr<Instance> instance;
  std::unique_ptr<Window> window;
  std::unique_ptr<Device> device;
  std::unique_ptr<Surface> surface;
  std::unique_ptr<GraphicsPipeline> graphicsPipeline;
  std::unique_ptr<SwapChain> swapChain;
  std::unique_ptr<PipelineLayout> pipelineLayout;
  std::vector<PipelineShaderStage> shaderStages;
  std::unique_ptr<CommandPool> commandPool;
  std::vector<std::unique_ptr<CommandBuffer>> commandBuffers;

  // TODO: Determine if we should allow for custom physical device selection.
  // This is initially just to select the default for the getting started

  size_t frameCount = 0;

  void pickDefaultPhysicalDevice();
  PipelineFixedFunctions GetFixedFunctions();
  std::vector<PipelineShaderStage> GetShaderStages();
  void CreatePipelineLayout();
};
} // namespace Vulkan

#endif