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

        const int imageWidth = 800;
        const int imageHeight = 600;
        Prism::Camera camera(
            Prism::Point3(0, 0, 1),   // Posição
            Prism::Point3(0, 0, 0),   // Alvo
            Prism::Vector3(0, 1, 0),  // Vetor 'up'
            1.0,                      // Distância da tela
            1.0,                      // Altura do viewport
            (double)imageWidth / imageHeight, // Largura do viewport (aspect ratio)
            imageHeight, imageWidth
        );

        Prism::VulkanRenderer renderer(vulkanContext, camera, imageWidth, imageHeight);

        auto start = std::chrono::high_resolution_clock::now();
        renderer.render();
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        Prism::Style::logInfo("GPU Render Time: " + std::to_string(elapsed.count()) + " ms");

        renderer.saveImage("output_gpu.ppm");


        // Prism::SceneParser("./data/input/scene.yml").parse().render();

    } catch (const std::exception& e) {
        Prism::Style::logError(e.what());
        return 1;
    }

    return 0;
}