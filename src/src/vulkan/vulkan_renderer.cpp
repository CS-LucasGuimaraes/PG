#include "Prism/vulkan/vulkan_renderer.hpp"

#include "Prism/core/style.hpp"

namespace Prism {

VulkanRenderer::VulkanRenderer(VulkanContext& context) : m_context(context) {
    try {
        // createOutputBuffer();
        // createDescriptorSetLayout();
        // createComputePipeline();
        // createCommandBuffer();
        Style::logInfo("VulkanRenderer initialized.");
    } catch (const vk::Error& e) {
        Style::logError("VulkanRenderer initialization failed: " + std::string(e.what()));
        throw;
    }
}

VulkanRenderer::~VulkanRenderer() {
    Style::logInfo("VulkanRenderer destroyed.");
}

void VulkanRenderer::render() {
    Style::logDone("Render pass initiated (placeholder).");
}

} // namespace Prism