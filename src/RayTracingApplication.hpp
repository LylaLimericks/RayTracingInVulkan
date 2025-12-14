#ifndef RAY_TRACING_APPLICATION_H
#define RAY_TRACING_APPLICATION_H

#include "WindowManager.hpp"
#include <vulkan/vulkan_raii.hpp>

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