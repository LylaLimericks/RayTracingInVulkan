#ifndef PIPELINE_STATES
#define PIPELINE_STATES

#include "ColorBlendState.hpp"
#include "InputAssemblyState.hpp"
#include "MultisampleState.hpp"
#include "RasterizationState.hpp"
#include "VertexInputState.hpp"
#include "ViewportState.hpp"

namespace Vulkan {

struct PipelineFixedFunctions {
  const ColorBlendState colorBlendState;
  const InputAssemblyState inputAssemblyState;
  const MultiSampleState multisampleState;
  const RasterizationState rasterizationState;
  const VertexInputState vertexInputState;
  const ViewportState viewportState;
};
} // namespace Vulkan

#endif