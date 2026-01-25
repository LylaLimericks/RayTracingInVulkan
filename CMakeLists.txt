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

find_program(SLANGC_EXECUTABLE slangc HINTS $ENV{VULKAN_SDK}/bin REQUIRED)
function (add_slang_shader_target TARGET)
        cmake_parse_arguments("SHADER" "" "" "SOURCES" ${ARGN})
        set(SHADERS_DIR shaders)
        set (ENTRY_POINTS -entry vertMain -entry fragMain)
        add_custom_command(
                OUTPUT ${SHADERS_DIR}
                COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADERS_DIR}
        )
        add_custom_command(
                OUTPUT ${SHADERS_DIR}/slang.spv
                COMMAND ${SLANGC_EXECUTABLE} ${SHADER_SOURCES} -target spirv -profile spirv_1_4 -emit-spirv-directly -fvk-use-entrypoint-name ${ENTRY_POINTS} -o slang.spv
                WORKING_DIRECTORY ${SHADERS_DIR}
                DEPENDS ${SHADERS_DIR} ${SHADER_SOURCES}
                COMMENT "Compiling Slang Shaders..."
                VERBATIM
        )
        add_custom_target(${TARGET} DEPENDS ${SHADERS_DIR}/slang.spv)
endfunction()

add_executable(RayTracer)
file (GLOB SHADER_SLANG_SOURCES src/shaders/*.slang)

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

add_slang_shader_target(shaders SOURCES ${SHADER_SLANG_SOURCES})
add_dependencies(RayTracer shaders)

target_compile_features(RayTracer PRIVATE cxx_std_20)

