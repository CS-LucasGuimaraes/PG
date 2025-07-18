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
    VulkanContext& m_context;
    uint32_t m_imageWidth;
    uint32_t m_imageHeight;


    vk::Buffer m_outputBuffer;
    VmaAllocation m_outputBufferAllocation;

    vk::DescriptorSetLayout m_descriptorSetLayout;
    vk::DescriptorPool m_descriptorPool;
    vk::DescriptorSet m_descriptorSet;

    vk::PipelineLayout m_pipelineLayout;
    vk::Pipeline m_computePipeline;


    vk::CommandPool m_commandPool;
    vk::CommandBuffer m_commandBuffer;
    vk::Fence m_fence;

    vk::Buffer m_cameraBuffer;
    VmaAllocation m_cameraBufferAllocation;

    vk::Buffer m_sphereBuffer;
    VmaAllocation m_sphereBufferAllocation;

    
    void createBuffers(const Camera& camera, const std::vector<GPUSphere>& spheres);
    void createDescriptorSet();
    void createComputePipeline();
    void createCommandObjects();

    void createCommandBuffer();

};

} // namespace Prism

#endif // PRISM_VULKAN_RENDERER_HPP_