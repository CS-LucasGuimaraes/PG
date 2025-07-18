#ifndef PRISM_VULKAN_GPU_STRUCTS_HPP_
#define PRISM_VULKAN_GPU_STRUCTS_HPP_

namespace Prism {

struct GPUCameraData {
    alignas(16) float position[3];
    alignas(16) float view_u[3];
    alignas(16) float view_v[3];
    alignas(16) float pixel_00_loc[3];
};

struct GPUMaterial {
    alignas(16) float color[3];
    alignas(16) float ambient[3];
    alignas(16) float specular[3];
    alignas(4) float shininess; // ns
    // Futuramente, adicionaremos ni, d, etc.
};

struct GPUObject {
    alignas(16) float center[3]; // Dados da Esfera
    alignas(4) float radius;     // Dados da Esfera

    // ... aqui entrarão dados de outros objetos, como a normal de um plano ...

    alignas(4) int type; // 0 = Esfera, 1 = Plano, etc.
    alignas(4) int material_index; // Índice para um array de materiais
};

} // namespace Prism

#endif // PRISM_VULKAN_GPU_STRUCTS_HPP_
