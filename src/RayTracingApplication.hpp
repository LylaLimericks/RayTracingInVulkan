#ifndef RAY_TRACING_APPLICATION_H
#define RAY_TRACING_APPLICATION_H

#ifdef NDEBUG
constexpr bool enableValidationLayers = false;
#else
constexpr bool enableValidationLayers = true;
#endif

#include "WindowManager.hpp"
#include <vulkan/vulkan_raii.hpp>

const std::vector<char const *> validationLayers = {
    "VK_LAYER_KHRONOS_validation",
};

class RayTracingApplication {
public:
  void run();
  void assignWindowManager(WindowManager &winManager);

private:
  void initVulkan();
  void mainLoop();
  void cleanup();

  WindowManager windowManager;
  vk::raii::Instance instance;
  vk::raii::Context context;

  void createInstance();
};

#endif