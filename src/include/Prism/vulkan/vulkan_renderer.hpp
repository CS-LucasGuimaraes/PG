#ifndef PRISM_VULKAN_RENDERER_HPP_
#define PRISM_VULKAN_RENDERER_HPP_

#include "prism_export.h"

#include "Prism/vulkan/vulkan_context.hpp"
#include "Prism/scene/camera.hpp"

namespace Prism {

class PRISM_EXPORT VulkanRenderer {
public:
    VulkanRenderer(VulkanContext& context, const Camera& camera, uint32_t imageWidth, uint32_t imageHeight);
    ~VulkanRenderer();

    void render();
    void saveImage(const std::string& filename);

private:
    // Vulkan context reference
    VulkanContext& m_context;
    uint32_t m_imageWidth;
    uint32_t m_imageHeight;

    // Vulkan resources
    vk::Buffer m_outputBuffer;
    VmaAllocation m_outputBufferAllocation;

    // Descriptor set and pipeline objects
    vk::DescriptorSetLayout m_descriptorSetLayout;
    vk::DescriptorPool m_descriptorPool;
    vk::DescriptorSet m_descriptorSet;

    vk::PipelineLayout m_pipelineLayout;
    vk::Pipeline m_computePipeline;

    // Command buffer and synchronization objects
    vk::CommandPool m_commandPool;
    vk::CommandBuffer m_commandBuffer;
    vk::Fence m_fence;

    // GPU Buffers
    vk::Buffer m_cameraBuffer;
    VmaAllocation m_cameraBufferAllocation;

    vk::Buffer m_objectBuffer;
    VmaAllocation m_objectBufferAllocation;

    vk::Buffer m_materialBuffer;
    VmaAllocation m_materialBufferAllocation;

    
    void createBuffers(const Camera& camera, const std::vector<GPUObject>& objects, const std::vector<GPUMaterial>& materials);
    void createDescriptorSet();
    void createComputePipeline();
    void createCommandObjects();

    void createCommandBuffer();

};

} // namespace Prism

#endif // PRISM_VULKAN_RENDERER_HPP_