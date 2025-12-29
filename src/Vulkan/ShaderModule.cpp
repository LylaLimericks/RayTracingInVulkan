#include "ShaderModule.hpp"
#include "Vulkan/Device.hpp"
#include "vulkan/vulkan.hpp"
#include <fstream>
#include <stdexcept>
#include <vulkan/vulkan_raii.hpp>

namespace Vulkan {

ShaderModule::ShaderModule(const Device &device, const std::string &filename) : ShaderModule(device, ReadFromFile(filename)) {
}

ShaderModule::ShaderModule(const Device &device, const std::vector<char> &code) : device(device) {
  vk::ShaderModuleCreateInfo createInfo{
      .codeSize = code.size() * sizeof(char),
      .pCode = reinterpret_cast<const uint32_t *>(code.data()),
  };

  shaderModule = std::make_unique<vk::raii::ShaderModule>(*device.Handle(), createInfo);
}

std::vector<char> ShaderModule::ReadFromFile(const std::string &filename) {
  std::ifstream shaderFile(filename, std::ios::ate | std::ios::binary);

  if (!shaderFile.is_open()) {
    throw std::runtime_error("Failed to load shader file!");
  }

  std::vector<char> buffer(shaderFile.tellg());
  shaderFile.seekg(0, std::ios::beg);
  shaderFile.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));
  shaderFile.close();

  return buffer;
}

} // namespace Vulkan
