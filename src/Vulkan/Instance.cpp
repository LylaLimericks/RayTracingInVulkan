#include "Instance.hpp"
#include "Vulkan/ApplicationInfo.hpp"
#include "Vulkan/Version.hpp"
#include "Vulkan/Window.hpp"
#include "vulkan/vulkan.hpp"
#include <algorithm>
#include <stdexcept>

namespace {
class Window;
inline constexpr bool IsSubsetOf(const std::vector<const char *> &requiredLayers, const std::vector<vk::LayerProperties> &supportedLayers) {
  if (std::ranges::any_of(requiredLayers, [&supportedLayers](auto const &requiredLayer) {
        return std::ranges::none_of(
            supportedLayers, [&requiredLayer](auto const &supportedLayer) {
              return strcmp(supportedLayer.layerName, requiredLayer);
            });
      })) {
    return false;
  }

  return true;
}

inline constexpr bool HasFullValidationLayerSupport(const std::vector<const char *> requiredLayers, const std::vector<vk::LayerProperties> supportedLayers) {
  return IsSubsetOf(requiredLayers, supportedLayers);
}

inline constexpr bool VulkanSupportsRequiredExtensions(const std::vector<const char *> requiredExtensions, const std::vector<vk::ExtensionProperties> supportedExtensions) {
  for (auto const &requiredExtension : requiredExtensions) {
    if (std::ranges::none_of(supportedExtensions, [requiredExtension](auto const &suppExt) {
          return strcmp(suppExt.extensionName, requiredExtension) == 0;
        })) {
      return false;
    }
  }

  return true;
}

// TODO: Refactor this into ApplicationInfo
inline constexpr vk::ApplicationInfo convertApplicationInfo(const Vulkan::ApplicationInfo &appInfo) {
  return vk::ApplicationInfo{
      .applicationVersion = VK_MAKE_VERSION(appInfo.AppVersion.Major, appInfo.AppVersion.Minor, appInfo.AppVersion.Patch),
      .pEngineName = appInfo.EngineName.c_str(),
      .engineVersion = VK_MAKE_VERSION(appInfo.EngineVersion.Major, appInfo.EngineVersion.Minor, appInfo.EngineVersion.Patch),
      .apiVersion = vk::ApiVersion14,
  };
}

} // namespace

namespace Vulkan {

Instance::Instance(const Window &window,
                   const ApplicationInfo &appInfo,
                   const std::vector<const char *> &requiredValidationLayers,
                   bool enableValidationLayers)
    : window(window), appInfo(appInfo), validationLayers(requiredValidationLayers) {

  // Ensure that we support all required validation layers
  auto supportedLayerProperties = context.enumerateInstanceLayerProperties();
  bool validationLayersAreEnabled = enableValidationLayers && !requiredValidationLayers.empty();

  if (validationLayersAreEnabled) {
    if (!HasFullValidationLayerSupport(requiredValidationLayers, supportedLayerProperties)) {
      throw new std::runtime_error("Required ValidationLayers not supported!"); // TODO: Return a catchable error instead of this (or do exception free)
    }
  }

  // Ensure that we support all required extensions for the provided window
  // TODO: Determine if we can just accept a list of extensions instead of the full window object.
  std::vector<const char *> reqExtensions = window.GetRequiredExtensions();

  if (validationLayersAreEnabled) {
    reqExtensions.push_back(vk::EXTDebugUtilsExtensionName);
  }

  const auto supportedExtensions = context.enumerateInstanceExtensionProperties();
  if (!VulkanSupportsRequiredExtensions(reqExtensions, supportedExtensions)) {
    throw new std::runtime_error("Required extensions not supported!"); // TODO: Return a catchable error instead of this (or do exception free)
  }

  const auto vkAppInfo = convertApplicationInfo(appInfo);
  const vk::InstanceCreateInfo createInfo{
      .pApplicationInfo = &vkAppInfo,
      .enabledLayerCount = static_cast<uint32_t>(requiredValidationLayers.size()),
      .ppEnabledLayerNames = requiredValidationLayers.data(),
      .enabledExtensionCount = static_cast<uint32_t>(reqExtensions.size()),
      .ppEnabledExtensionNames = reqExtensions.data(),
  };

  instance = vk::raii::Instance(context, createInfo);
}

const std::vector<vk::raii::PhysicalDevice> Instance::PhysicalDevices() const {
  return instance.enumeratePhysicalDevices();
}

} // namespace Vulkan