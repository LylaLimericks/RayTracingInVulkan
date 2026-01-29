#ifndef SHADER_MODULE_H
#define SHADER_MODULE_H

#include <string>
#include <vector>
#include <vulkan/vulkan_raii.hpp>
namespace Vulkan {

class VulkanDevice;

class ShaderModule {
public:
  ShaderModule(const VulkanDevice &device, const std::string &filename);
  ShaderModule(const VulkanDevice &device, const std::vector<char> &code);
  ~ShaderModule();

  const vk::ShaderModule Handle() const { return shaderModule; }

private:
  const class VulkanDevice &device;
  vk::raii::ShaderModule shaderModule = nullptr;

  static std::vector<char> ReadFromFile(const std::string &filename);
};

} // namespace Vulkan

#endif