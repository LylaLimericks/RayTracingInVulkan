#include "RayTracingApplication.hpp"
#include "Vulkan/Device.hpp"
#include "Vulkan/SwapChain.hpp"
#include "Vulkan/Window.hpp"
#include "vulkan/vulkan.hpp"
#include <cstdint>
#include <stdexcept>
#include <vulkan/vulkan_raii.hpp>
namespace Implementations {

void RayTracingApplication::run() {
}

void RayTracingApplication::mainLoop() {
  while (!window->shouldClose()) {
    window->pollEvents();
    drawFrame();
  }
}

void RayTracingApplication::drawFrame() {
  auto fenceResult = device->Handle()->waitForFences(*drawFence, vk::True, 999999);

  auto [result, imageIndex] = swapChain->AcquireNextFrame(UINT64_MAX, *presentCompleteSemaphores[currentFrame]);

  if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR) {
    throw std::runtime_error("Failed to acquire swap chain image!");
  }

  vk::PipelineStageFlags waitDestinationStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);

  const vk::SubmitInfo submitInfo {
    .waitSemaphoreCount = 1,
    .pWaitSemaphores = &*presentCompleteSemaphores[currentFrame],
    .pWaitDstStageMask = &waitDestinationStageMask,
    .commandBufferCount = 1,
    .pCommandBuffers = &*commandBuffers[currentFrame]->Buffer(),
    .signalSemaphoreCount = 1,
    .pSignalSemaphores = &*renderFinishedSemaphores[imageIndex]
  }
}

void RayTracingApplication::createSyncObjects() {
  presentCompleteSemaphores.clear();
  renderFinishedSemaphores.clear();

  for (int i = 0; i < frameCount; i++) {
    presentCompleteSemaphores.emplace_back(*device->Handle(), vk::SemaphoreCreateInfo());
    renderFinishedSemaphores.emplace_back(*device->Handle(), vk::SemaphoreCreateInfo());
  }

  drawFence = vk::raii::Fence(*device->Handle(), {.flags = vk::FenceCreateFlagBits::eSignaled});
}

void RayTracingApplication::recordCommandBuffer(uint32_t imageIndex) {
  commandBuffers->Begin({});
  commandBuffer->TransitionImageLayout(
      swapChain->Image(imageIndex),
      vk::ImageLayout::eUndefined,
      vk::ImageLayout::eColorAttachmentOptimal,
      {},
      vk::AccessFlagBits2::eColorAttachmentWrite,
      vk::PipelineStageFlagBits2::eColorAttachmentOutput,
      vk::PipelineStageFlagBits2::eColorAttachmentOutput);

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

  auto cBuff = commandBuffer->Buffer();

  cBuff->beginRendering(renderingInfo);

  commandBuffer->BindPipeline(vk::PipelineBindPoint::eGraphics, *graphicsPipeline);
  cBuff->setViewport(0, vk::Viewport(0.0f, 0.0f, static_cast<float>(swapChain->Extent().width), static_cast<float>(swapChain->Extent().height), 0.0f, 1.0f));
  cBuff->setScissor(0, vk::Rect2D(vk::Offset2D(0, 0), swapChain->Extent()));

  cBuff->draw(3, 1, 0, 0);

  cBuff->endRendering();

  commandBuffer->TransitionImageLayout(
      swapChain->Image(imageIndex),
      vk::ImageLayout::eColorAttachmentOptimal,
      vk::ImageLayout::ePresentSrcKHR,
      vk::AccessFlagBits2::eColorAttachmentWrite,
      {},
      vk::PipelineStageFlagBits2::eColorAttachmentOutput,
      vk::PipelineStageFlagBits2::eBottomOfPipe);

  cBuff->end();
}
} // namespace Implementations