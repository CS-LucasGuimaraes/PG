#include "Prism/vulkan/vulkan_context.hpp"

#include "Prism/core/style.hpp"

#include <iostream>

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE;

#define VMA_IMPLEMENTATION
#include <vma/vk_mem_alloc.h>

namespace Prism {

VulkanContext::VulkanContext() {
    try {
        VULKAN_HPP_DEFAULT_DISPATCHER.init();

        createInstance();
        pickPhysicalDevice();
        createLogicalDevice();
        createAllocator();
        Style::logInfo("Vulkan context initialized successfully.");
    } catch (const vk::Error& e) {
        Style::logError("Vulkan initialization failed: " + std::string(e.what()));
        throw; // Re-throw the exception
    }
}

VulkanContext::~VulkanContext() {
    if (m_allocator) {
        vmaDestroyAllocator(m_allocator);
    }
    if (m_device) {
        m_device.destroy();
    }
    if (m_instance) {
        m_instance.destroy();
    }
    Style::logInfo("Vulkan context destroyed.");
}

void VulkanContext::createInstance() {
    vk::ApplicationInfo appInfo("Prism", 1, "Prism Render Engine", 1, VK_API_VERSION_1_2);
    vk::InstanceCreateInfo createInfo({}, &appInfo);

    m_instance = vk::createInstance(createInfo);

    VULKAN_HPP_DEFAULT_DISPATCHER.init(m_instance);

    Style::logInfo("Vulkan Instance created.");
}

void VulkanContext::pickPhysicalDevice() {
    // For now, we'll just pick the first available device.
    std::vector<vk::PhysicalDevice> devices = m_instance.enumeratePhysicalDevices();
    if (devices.empty()) {
        throw std::runtime_error("Failed to find GPUs with Vulkan support!");
    }
    m_physicalDevice = devices[0];

    vk::PhysicalDeviceProperties properties = m_physicalDevice.getProperties();
    Style::logInfo("Picked GPU: " + std::string(properties.deviceName));
}

void VulkanContext::createLogicalDevice() {
    // We need to find a queue family that supports compute operations
    auto queueFamilyProperties = m_physicalDevice.getQueueFamilyProperties();

    m_computeQueueFamilyIndex = -1;

    for (uint32_t i = 0; i < queueFamilyProperties.size(); ++i) {
        if (queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eCompute) {
            m_computeQueueFamilyIndex = i;
            break;
        }
    }

    if (m_computeQueueFamilyIndex < 0) {
        throw std::runtime_error("Failed to find a queue family that supports compute!");
    }

    float queuePriority = 1.0f;
    vk::DeviceQueueCreateInfo queueCreateInfo({}, m_computeQueueFamilyIndex, 1, &queuePriority);
    vk::DeviceCreateInfo deviceCreateInfo({}, 1, &queueCreateInfo);

    m_device = m_physicalDevice.createDevice(deviceCreateInfo);

    // VULKAN_HPP_DEFAULT_DISPATCHER.init(m_device);

    m_computeQueue = m_device.getQueue(m_computeQueueFamilyIndex, 0); // Get the first queue
    Style::logInfo("Vulkan Logical Device and Compute Queue created.");
}

void VulkanContext::createAllocator() {
    VmaVulkanFunctions vulkanFunctions = {};
    vulkanFunctions.vkGetInstanceProcAddr = VULKAN_HPP_DEFAULT_DISPATCHER.vkGetInstanceProcAddr;
    vulkanFunctions.vkGetDeviceProcAddr = VULKAN_HPP_DEFAULT_DISPATCHER.vkGetDeviceProcAddr;

    VmaAllocatorCreateInfo allocatorInfo = {};
    allocatorInfo.vulkanApiVersion = VK_API_VERSION_1_2;
    allocatorInfo.physicalDevice = m_physicalDevice;
    allocatorInfo.device = m_device;
    allocatorInfo.instance = m_instance;
    allocatorInfo.pVulkanFunctions = &vulkanFunctions;

    vmaCreateAllocator(&allocatorInfo, &m_allocator);
    Style::logInfo("Vulkan Memory Allocator (VMA) created.");
}

} // namespace Prism