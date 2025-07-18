#ifndef PRISM_VULKAN_RENDERER_HPP_
#define PRISM_VULKAN_RENDERER_HPP_

#include "prism_export.h"

#include "Prism/vulkan/vulkan_context.hpp"

namespace Prism {

class PRISM_EXPORT VulkanRenderer {
public:
    VulkanRenderer(VulkanContext& context);
    ~VulkanRenderer();

    void render();

private:
    VulkanContext& m_context;

    void createOutputBuffer();
    void createDescriptorSetLayout();
    void createComputePipeline();
    void createCommandBuffer();

};

} // namespace Prism

#endif // PRISM_VULKAN_RENDERER_HPP_