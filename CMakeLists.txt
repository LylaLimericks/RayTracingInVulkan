cmake_minimum_required(VERSION 3.29)

project(RayTracingInVulkan)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake")

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

find_package(Vulkan REQUIRED)
add_library(VulkanCppModule INTERFACE)
add_library(Vulkan::cppm ALIAS VulkanCppModule)
target_link_libraries(VulkanCppModule INTERFACE Vulkan::Vulkan)

target_compile_definitions(VulkanCppModule INTERFACE
  VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1
  VULKAN_HPP_NO_STRUCT_CONSTRUCTORS=1
)

add_library(Application)

target_sources(Application
  PRIVATE
    src/Vulkan/Application.cpp
    src/Vulkan/GraphicsPipeline.cpp
    src/Vulkan/ImageView.cpp
    src/Vulkan/Instance.cpp
    src/Vulkan/PipelineLayout.cpp
    src/Vulkan/ShaderModule.cpp
    src/Vulkan/Surface.cpp
    src/Vulkan/SwapChain.cpp
    src/Vulkan/VulkanDevice.cpp
    src/Vulkan/Window.cpp
    src/Vulkan/PipelineShaderStages/PipelineShaderStage.cpp
    src/RayTracingApplication.cpp
    src/Vulkan/Surface.cpp
    src/Vulkan/Window.cpp

  PUBLIC
    FILE_SET applicationHeaders
    TYPE HEADERS
    BASE_DIRS
      src
    FILES
    src/Vulkan/Application.hpp
    src/Vulkan/ApplicationInfo.hpp
    src/Vulkan/GraphicsPipeline.hpp
    src/Vulkan/ImageView.hpp
    src/Vulkan/Instance.hpp
    src/Vulkan/PipelineLayout.hpp
    src/Vulkan/ShaderModule.hpp
    src/Vulkan/Surface.hpp
    src/Vulkan/SwapChain.hpp
    src/Vulkan/Version.hpp
    src/Vulkan/VulkanDevice.hpp
    src/Vulkan/Window.hpp
    src/Vulkan/WindowConfig.hpp
    src/Vulkan/FixedFunctions/ColorBlendState.hpp
    src/Vulkan/FixedFunctions/FixedFunction.hpp
    src/Vulkan/FixedFunctions/InputAssemblyState.hpp
    src/Vulkan/FixedFunctions/MultisampleState.hpp
    src/Vulkan/FixedFunctions/PipelineFixedFunctions.hpp
    src/Vulkan/FixedFunctions/RasterizationState.hpp
    src/Vulkan/FixedFunctions/VertexInputState.hpp
    src/Vulkan/FixedFunctions/ViewportState.hpp
    src/Vulkan/PipelineShaderStages/FragmentShaderStage.hpp
    src/Vulkan/PipelineShaderStages/PipelineShaderStage.hpp
    src/Vulkan/PipelineShaderStages/VertexShaderStage.hpp
    src/RayTracingApplication.hpp
)

target_compile_features(Application PRIVATE cxx_std_20)
target_compile_definitions(Application PRIVATE VULKAN_HPP_NO_STRUCT_CONSTRUCTORS)

# add_library(VulkanRT)

# file (GLOB VULKAN_RT_SOURCES src/Vulkan/*.cpp)

# file (GLOB VULKAN_RT_HEADERS src/Vulkan/*.hpp)

# target_sources(VulkanRT
#   PRIVATE
#     ${VULKAN_RT_SOURCES}

#   PUBLIC
#     FILE_SET vulkanRtHeaders
#     TYPE HEADERS
#     BASE_DIRS
#       src/Vulkan
#     FILES
#       ${VULKAN_RT_HEADERS}
# )

add_executable(RayTracer)

target_sources(RayTracer
  PRIVATE
    main.cpp
)

target_link_libraries(RayTracer
  PRIVATE
    Vulkan::cppm
    Application
    glfw
    # VulkanRT
)

target_compile_features(RayTracer PRIVATE cxx_std_20)

