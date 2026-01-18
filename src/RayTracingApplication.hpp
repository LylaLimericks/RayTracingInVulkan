#pragma once
#include "Vulkan/Application.hpp"
#include "Vulkan/ApplicationInfo.hpp"
#include "Vulkan/Version.hpp"
#include "Vulkan/WindowConfig.hpp"
#include "vulkan/vulkan.hpp"
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_raii.hpp>
#include <vulkan/vulkan_to_string.hpp>

namespace Implementations {

#ifdef NDEBUG
constexpr bool enableValidationLayers = false;
#else
constexpr bool enableValidationLayers = true;
#endif

const std::vector<char const *> validationLayers = {
    "VK_LAYER_KHRONOS_validation",
};

constexpr Vulkan::WindowConfig windowConfig{
    .Title = "Ray Tracing App",
    .Width = 600,
    .Height = 400,
};

constexpr Vulkan::ApplicationInfo appInfo{
    .AppVersion = Vulkan::Version{.Major = 0, .Minor = 0, .Patch = 0},
    .EngineVersion = Vulkan::Version{.Major = 0, .Minor = 0, .Patch = 0},
    .EngineName = "None",
};

class RayTracingApplication : public Vulkan::Application {
public:
  RayTracingApplication() : Vulkan::Application(appInfo, windowConfig, vk::PresentModeKHR::eMailbox, enableValidationLayers) {}
  void run();

private:
  void mainLoop();
  void createSyncObjects();
  void drawFrame();
  void cleanUp();
  void recordCommandBuffer(const size_t currentFrame, const uint32_t imageIndex);

  std::vector<vk::raii::Semaphore> presentCompleteSemaphores;
  std::vector<vk::raii::Semaphore> renderFinishedSemaphores;
  std::vector<vk::raii::Fence> inFlightFences;
  uint32_t currentFrame = 0;
};

} // namespace Implementations