#ifndef SHADER_MODULE_H
#define SHADER_MODULE_H

#include <string>
#include <vector>
#include <vulkan/vulkan_raii.hpp>
namespace Vulkan {

class Device;

class ShaderModule {
public:
  ShaderModule(const Device &device, const std::string &filename);
  ShaderModule(const Device &device, const std::vector<char> &code);
  ~ShaderModule();

  const vk::raii::ShaderModule *Handle() const { return shaderModule.get(); }

private:
  const class Device &device;
  std::unique_ptr<vk::raii::ShaderModule> shaderModule;

  static std::vector<char> ReadFromFile(const std::string &filename);
};

} // namespace Vulkan

#endif