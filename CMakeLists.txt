cmake_minimum_required(VERSION 3.29)

project(RayTracingInVulkan)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake")

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

find_package(Vulkan REQUIRED)
add_library(VulkanCppModule INTERFACE)
add_library(Vulkan::cppm ALIAS VulkanCppModule)

target_compile_definitions(VulkanCppModule INTERFACE
  VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1
  VULKAN_HPP_NO_STRUCT_CONSTRUCTORS=1
)

add_library(Application)

target_sources(Application
  PRIVATE
    src/RayTracingApplication.cpp
    src/WindowManager.cpp

  PUBLIC
    FILE_SET applicationHeaders
    TYPE HEADERS
    BASE_DIRS
      src
    FILES
      src/RayTracingApplication.hpp
      src/WindowManager.hpp
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

