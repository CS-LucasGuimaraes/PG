#ifndef PRISM_VULKAN_CONTEXT_HPP_
#define PRISM_VULKAN_CONTEXT_HPP_

#include "prism_export.h"

#include "Prism/vulkan/vulkan_gpu_structs.hpp"

#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan.hpp>

namespace Prism {

class PRISM_EXPORT VulkanContext {
public:
    VulkanContext();
    ~VulkanContext();

    // Disable copying and moving for simplicity
    VulkanContext(const VulkanContext&) = delete;
    VulkanContext& operator=(const VulkanContext&) = delete;

    VmaAllocator getAllocator() const { return m_allocator; }
    vk::Device getDevice() const { return m_device; }
    vk::Queue getComputeQueue() const { return m_computeQueue; }
    uint32_t getComputeQueueFamilyIndex() const { return m_computeQueueFamilyIndex; }

private:
    void createInstance();
    void pickPhysicalDevice();
    void createLogicalDevice();
    void createAllocator();

    vk::Instance m_instance;
    vk::PhysicalDevice m_physicalDevice;
    vk::Device m_device;
    VmaAllocator m_allocator;
    vk::Queue m_computeQueue;
    uint32_t m_computeQueueFamilyIndex;
};

} // namespace Prism

#endif // PRISM_VULKAN_CONTEXT_HPP_