/**
 * @file main.cpp
 * @brief Prism Render Engine
 *
 * @copyright Copyright (c) 2025 src-lua
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "Prism.hpp"
#include "Prism/vulkan/vulkan_context.hpp"
#include "Prism/vulkan/vulkan_renderer.hpp"
#include <iostream>

int main() {
    try {
        Prism::VulkanContext vulkanContext;
        Prism::VulkanRenderer renderer(vulkanContext);

        // 3. Chame a função de renderização
        renderer.render();

        // Prism::SceneParser("./data/input/scene.yml").parse().render();

    } catch (const std::exception& e) {
        Prism::Style::logError(e.what());
        return 1;
    }

    return 0;
}