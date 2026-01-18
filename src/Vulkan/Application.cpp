#include "Application.hpp"
#include "Vulkan/FixedFunctions/ColorBlendState.hpp"
#include "Vulkan/FixedFunctions/InputAssemblyState.hpp"
#include "Vulkan/FixedFunctions/MultisampleState.hpp"
#include "Vulkan/FixedFunctions/PipelineFixedFunctions.hpp"
#include "Vulkan/FixedFunctions/RasterizationState.hpp"
#include "Vulkan/FixedFunctions/ViewportState.hpp"
#include "Vulkan/GraphicsPipeline.hpp"
#include "Vulkan/Instance.hpp"
#include "Vulkan/PipelineLayout.hpp"
#include "Vulkan/PipelineShaderStages/FragmentShaderStage.hpp"
#include "Vulkan/PipelineShaderStages/PipelineShaderStage.hpp"
#include "Vulkan/PipelineShaderStages/VertexShaderStage.hpp"
#include "Vulkan/ShaderModule.hpp"
#include "Vulkan/Surface.hpp"
#include "Vulkan/SwapChain.hpp"
#include "Vulkan/VulkanDevice.hpp"
#include "Window.hpp"
#include "vulkan/vulkan.hpp"
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_raii.hpp>

namespace {

vk::raii::PhysicalDevice pickPhysicalDevice(const std::vector<vk::raii::PhysicalDevice> &devices, const std::vector<const char *> requiredExtensions) {
  if (devices.empty()) {
    throw std::runtime_error("Failed to find GPUs with Vulkan Support!");
  }

  // Iterate through the devices to find a suitable candidate
  for (const auto &device : devices) {
    bool isSuitable = device.getProperties().apiVersion >= VK_API_VERSION_1_3;
    if (!isSuitable) {
      continue;
    }

    const auto queueFamilies = device.getQueueFamilyProperties();

    // Verify the existence of a queue family that supports graphics
    for (const auto &queueFamilyProps : queueFamilies) {
      if ((queueFamilyProps.queueFlags & vk::QueueFlagBits::eGraphics) !=
          static_cast<vk::QueueFlags>(0)) {
        isSuitable = false;
        break;
      }
    }

    // Vertify that the device supports all required extensions
    auto extensions = device.enumerateDeviceExtensionProperties();
    bool foundAllExtensions = true;
    for (const auto &reqExtension : requiredExtensions) {
      bool foundExtension = false;
      for (const auto &extension : extensions) {
        if (strcmp(extension.extensionName, reqExtension) == 0) {
          foundExtension = true;
          break;
        }
      }
      foundAllExtensions = foundAllExtensions & foundExtension;
    }

    isSuitable = isSuitable && foundAllExtensions;
    if (isSuitable) {
      return device;
    }
  }

  throw std::runtime_error("Failed to find a suitable GPU!"); // TODO: Convert this over to no longer rely on exception handling.
}

} // namespace

namespace Vulkan {

Application::Application(const ApplicationInfo &appInfo,
                         const WindowConfig &windowConfig,
                         const vk::PresentModeKHR presentMode,
                         bool enableValidationLayers) : presentMode(presentMode) {
  const auto validationLayers =
      enableValidationLayers
          ? std::vector<const char *>{"VK_LAYER_KHRONOS_validation"}
          : std::vector<const char *>();

  window.reset(new Window(windowConfig));
  instance.reset(new Instance(*window, appInfo, validationLayers, enableValidationLayers));
  pickDefaultPhysicalDevice();
  surface.reset(new Surface(*instance, *window));
  createSwapChain();
  frameCount = swapChain->Size();
  commandBuffers = device->CreateCommandBuffers(vk::CommandBufferLevel::ePrimary, frameCount);
}

void Application::pickDefaultPhysicalDevice() {
  const auto devices = instance->PhysicalDevices();

  const std::vector<const char *> deviceExtensions{
      vk::KHRSwapchainExtensionName,
      vk::KHRSpirv14ExtensionName,
      vk::KHRSynchronization2ExtensionName,
      vk::KHRCreateRenderpass2ExtensionName,
  };
  const auto pickedDevice = pickPhysicalDevice(devices, deviceExtensions);

  vk::StructureChain<vk::PhysicalDeviceFeatures2,
                     vk::PhysicalDeviceVulkan13Features,
                     vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>
      featureChain = {
          {}, // vk::PhysicalDeviceFeatures2 (leaving empty for now)
          {.dynamicRendering =
               true}, // Enable dynamic rendering from Vulkan 1.3
          {.extendedDynamicState =
               true} // Enable extended dynamic state from the extension.
      };

  const vk::PhysicalDeviceFeatures deviceFeatures{};
  physicalDevice = pickedDevice;
  device.reset(new VulkanDevice(pickedDevice, *surface->Handle(), deviceExtensions, deviceFeatures, &featureChain));
}

void Application::createSwapChain() {
  swapChain.reset(new SwapChain(physicalDevice, *device.get(), *window.get(), *surface->Handle(), vk::PresentModeKHR::eMailbox));
  const auto fixedFunctions = GetFixedFunctions();
  const auto shaderStages = GetShaderStages();
  CreatePipelineLayout();
  graphicsPipeline.reset(new GraphicsPipeline(*device.get(), *swapChain, fixedFunctions, shaderStages, *pipelineLayout));
}

PipelineFixedFunctions Application::GetFixedFunctions() {
  ///
  // Vertex Input
  ///
  vk::PipelineVertexInputStateCreateInfo vertexInputInfo; // Leaving null for now as we're not currently loading verts
  VertexInputState vertexInput(vertexInputInfo);

  ///
  // Input Assembly
  ///
  vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo{
      .topology = vk::PrimitiveTopology::eTriangleList,
  };
  InputAssemblyState inputAssembly(inputAssemblyInfo);

  ///
  // Viewport
  ///
  vk::PipelineViewportStateCreateInfo viewportStateInfo{
      .viewportCount = 1,
      .scissorCount = 1,
  };
  ViewportState viewportState(viewportStateInfo);

  ///
  // Rasterizer
  ///
  vk::PipelineRasterizationStateCreateInfo rasterizerInfo{
      .depthClampEnable = vk::False,
      .rasterizerDiscardEnable = vk::False,
      .polygonMode = vk::PolygonMode::eFill,
      .cullMode = vk::CullModeFlagBits::eBack,
      .frontFace = vk::FrontFace::eClockwise,
      .depthBiasClamp = vk::False,
      .depthBiasSlopeFactor = 1.0f,
      .lineWidth = 1.0f,
  };
  RasterizationState rasterizer(rasterizerInfo);

  ///
  // Multisampling
  ///
  vk::PipelineMultisampleStateCreateInfo multisamplerInfo{
      .rasterizationSamples = vk::SampleCountFlagBits::e1,
      .sampleShadingEnable = vk::False,
  };
  MultiSampleState multisampler(multisamplerInfo);

  ///
  // Color Blending
  ///
  vk::PipelineColorBlendAttachmentState colorBlendAttachment{
      .blendEnable = vk::False,
      .colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA,
  };

  vk::PipelineColorBlendStateCreateInfo colorBlendingInfo{
      .logicOpEnable = vk::False,
      .logicOp = vk::LogicOp::eCopy,
      .attachmentCount = 1,
      .pAttachments = &colorBlendAttachment,
  };
  ColorBlendState colorBlend(colorBlendingInfo);

  return PipelineFixedFunctions{
      .colorBlendState = colorBlend,
      .inputAssemblyState = inputAssembly,
      .multisampleState = multisampler,
      .rasterizationState = rasterizer,
      .vertexInputState = vertexInput,
      .viewportState = viewportState,
  };
}

void Application::CreatePipelineLayout() {
  vk::PipelineLayoutCreateInfo pipelineLayoutInfo{
      .setLayoutCount = 0,
      .pushConstantRangeCount = 0,
  };

  pipelineLayout.reset(new PipelineLayout(*device.get(), pipelineLayoutInfo));
}

std::vector<PipelineShaderStage> Application::GetShaderStages() {
  ShaderModule shaderModule(*device.get(), ShaderModules[0]);

  VertexShaderStage vertShader(shaderModule, "vertMain");
  FragmentShaderStage fragShader(shaderModule, "fragMain");

  std::vector<PipelineShaderStage> ret{
      vertShader,
      fragShader,
  };

  return ret;
}

void Application::TransitionImageLayout(
    const vk::Image &image,
    const vk::ImageLayout &oldLayout,
    const vk::ImageLayout &newLayout,
    const vk::AccessFlags2 &srcAccessMask,
    const vk::AccessFlags2 &dstAccessMask,
    const vk::PipelineStageFlags2 &srcStageMask,
    const vk::PipelineStageFlags2 &dstStageMask,
    const vk::ImageAspectFlags &imageAspectFlags,
    const uint32_t &currentFrame) const {
  vk::ImageMemoryBarrier2 barrier = {
      .srcStageMask = srcStageMask,
      .srcAccessMask = srcAccessMask,
      .dstStageMask = dstStageMask,
      .dstAccessMask = dstAccessMask,
      .oldLayout = oldLayout,
      .newLayout = newLayout,
      .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      .image = image,
      .subresourceRange = {
          .aspectMask = imageAspectFlags,
          .baseMipLevel = 0,
          .levelCount = 1,
          .baseArrayLayer = 0,
          .layerCount = 1}};

  vk::DependencyInfo dependencyInfo = {
      .dependencyFlags = {},
      .imageMemoryBarrierCount = 1,
      .pImageMemoryBarriers = &barrier};

  commandBuffers[currentFrame].pipelineBarrier2(dependencyInfo);
}

Application::~Application() {}

} // namespace Vulkan