#ifndef RAY_TRACING_APPLICATION_H
#define RAY_TRACING_APPLICATION_H

#include "Vulkan/Device.hpp"
#include "WindowManager.hpp"
#include <iostream>
#include <memory>
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_raii.hpp>
#include <vulkan/vulkan_to_string.hpp>

namespace RayTracing {

#ifdef NDEBUG
constexpr bool enableValidationLayers = false;
#else
constexpr bool enableValidationLayers = true;
#endif

const std::vector<char const *> validationLayers = {
    "VK_LAYER_KHRONOS_validation",
};

// TODO: Refactor these to no longer be compile time constants.
constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;

class RayTracingApplication {
public:
  void run();
  void assignWindowManager(WindowManager &winManager);
  void setPhysicalDevice();

private:
  void initVulkan();
  void mainLoop();
  void cleanup();

  WindowManager windowManager;
  Vulkan::Device device;
  vk::raii::Context context;
  std::unique_ptr<vk::raii::Instance> instance;
  std::unique_ptr<vk::raii::DebugUtilsMessengerEXT> debugMessenger;

  std::vector<const char *> deviceExtensions = {
      vk::KHRSwapchainExtensionName, vk::KHRSpirv14ExtensionName,
      vk::KHRSynchronization2ExtensionName,
      vk::KHRCreateRenderpass2ExtensionName};

  void createInstance();
  std::vector<const char *> getRequiredExtensions();
  void setupDebugMessenger();

  void pickPhysicalDevice();

  static VKAPI_ATTR vk::Bool32 VKAPI_CALL debugCallback(
      vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
      vk::DebugUtilsMessageTypeFlagsEXT type,
      const vk::DebugUtilsMessengerCallbackDataEXT *pCallbackData, void *) {
    std::cerr << "validation layer: type " << vk::to_string(type)
              << " msg: " << pCallbackData->pMessage << std::endl;

    return vk::False;
  }
};

} // namespace RayTracing
#endif