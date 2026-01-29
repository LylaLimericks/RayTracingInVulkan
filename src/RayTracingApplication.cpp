#include "RayTracingApplication.hpp"
#include "Vulkan/GraphicsPipeline.hpp"
#include "Vulkan/SwapChain.hpp"
#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/Window.hpp"
#include "vulkan/vulkan.hpp"
#include <cstdint>
#include <stdexcept>
#include <vulkan/vulkan_raii.hpp>
namespace Implementations {

void RayTracingApplication::run() {
  createSyncObjects();
  mainLoop();
}

void RayTracingApplication::mainLoop() {
  while (!window->shouldClose()) {
    window->pollEvents();
    drawFrame();
  }

  device->WaitIdle();
}

void RayTracingApplication::drawFrame() {
  auto fenceResult = device->WaitForFences(*inFlightFences[currentFrame], vk::True, 999999);

  auto [result, imageIndex] = swapChain->AcquireNextFrame(UINT64_MAX, *presentCompleteSemaphores[currentFrame]);

  if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR) {
    throw std::runtime_error("Failed to acquire swap chain image!");
  }

  device->ResetFences(*inFlightFences[currentFrame]);
  commandBuffers[currentFrame].reset();
  recordCommandBuffer(currentFrame, imageIndex);

  vk::PipelineStageFlags waitDestinationStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);

  const vk::SubmitInfo submitInfo{
      .waitSemaphoreCount = 1,
      .pWaitSemaphores = &*presentCompleteSemaphores[currentFrame],
      .pWaitDstStageMask = &waitDestinationStageMask,
      .commandBufferCount = 1,
      .pCommandBuffers = &*commandBuffers[currentFrame],
      .signalSemaphoreCount = 1,
      .pSignalSemaphores = &*renderFinishedSemaphores[imageIndex],
  };

  device->GraphicsQueue().submit(submitInfo, inFlightFences[currentFrame]);
  vk::PresentInfoKHR presentInfo = swapChain->GetPresentInfo(1, &*renderFinishedSemaphores[currentFrame], &imageIndex);

  presentInfo.pResults = nullptr;

  result = device->PresentQueue().presentKHR(presentInfo);
}

void RayTracingApplication::createSyncObjects() {
  presentCompleteSemaphores.clear();
  renderFinishedSemaphores.clear();

  for (int i = 0; i < frameCount; i++) {
    presentCompleteSemaphores.emplace_back(device->CreateSemaphore(vk::SemaphoreCreateInfo()));
    renderFinishedSemaphores.emplace_back(device->CreateSemaphore(vk::SemaphoreCreateInfo()));
  }

  for (size_t i = 0; i < frameCount; i++) {
    inFlightFences.emplace_back(device->CreateFence({.flags = vk::FenceCreateFlagBits::eSignaled}));
  }
}

void RayTracingApplication::recordCommandBuffer(const size_t currentFrame, const uint32_t imageIndex) {
  commandBuffers[currentFrame].begin({});
  TransitionImageLayout(
      swapChain->Image(imageIndex),
      vk::ImageLayout::eUndefined,
      vk::ImageLayout::eColorAttachmentOptimal,
      {},
      vk::AccessFlagBits2::eColorAttachmentWrite,
      vk::PipelineStageFlagBits2::eColorAttachmentOutput,
      vk::PipelineStageFlagBits2::eColorAttachmentOutput,
      vk::ImageAspectFlagBits::eColor,
      currentFrame);

  vk::ClearValue clearColor = vk::ClearColorValue(0.0f, 0.0f, 0.0f, 1.0f);
  vk::RenderingAttachmentInfo attachmentInfo = {
      .imageView = swapChain->ImageView(imageIndex),
      .imageLayout = vk::ImageLayout::eColorAttachmentOptimal,
      .loadOp = vk::AttachmentLoadOp::eClear,
      .storeOp = vk::AttachmentStoreOp::eStore,
      .clearValue = clearColor,
  };

  vk::RenderingInfo renderingInfo = {
      .renderArea = {.offset = {0, 0}, .extent = swapChain->Extent()},
      .layerCount = 1,
      .colorAttachmentCount = 1,
      .pColorAttachments = &attachmentInfo,
  };

  commandBuffers[currentFrame].beginRendering(renderingInfo);

  commandBuffers[currentFrame].bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline->Handle());
  commandBuffers[currentFrame].setViewport(0, vk::Viewport(0.0f, 0.0f, static_cast<float>(swapChain->Extent().width), static_cast<float>(swapChain->Extent().height), 0.0f, 1.0f));
  commandBuffers[currentFrame].setScissor(0, vk::Rect2D(vk::Offset2D(0, 0), swapChain->Extent()));

  commandBuffers[currentFrame].draw(3, 1, 0, 0);

  commandBuffers[currentFrame].endRendering();

  TransitionImageLayout(
      swapChain->Image(imageIndex),
      vk::ImageLayout::eColorAttachmentOptimal,
      vk::ImageLayout::ePresentSrcKHR,
      vk::AccessFlagBits2::eColorAttachmentWrite,
      {},
      vk::PipelineStageFlagBits2::eColorAttachmentOutput,
      vk::PipelineStageFlagBits2::eBottomOfPipe,
      vk::ImageAspectFlagBits::eColor,
      currentFrame);

  commandBuffers[currentFrame].end();
}
} // namespace Implementations