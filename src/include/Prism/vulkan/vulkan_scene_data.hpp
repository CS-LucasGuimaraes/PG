#ifndef PRISM_VULKAN_SCENE_DATA_HPP_
#define PRISM_VULKAN_SCENE_DATA_HPP_

#include "prism_export.h"
#include "Prism/scene/scene.hpp"
#include "Prism/vulkan/vulkan_gpu_structs.hpp"
#include <vector>
#include <map>
#include <memory>

namespace Prism {

/**
 * @class SceneDataGPU
 * @brief Converte e armazena os dados de uma cena em um formato pronto para a GPU.
 * * Esta classe pega um objeto Scene, processa seus objetos, materiais e luzes,
 * e os converte em arrays de structs (GPUObject, GPUMaterial, etc.) que podem
 * ser diretamente copiados para os buffers da GPU.
 */
class PRISM_EXPORT SceneDataGPU {
public:
    // Construtor que realiza a conversão
    explicit SceneDataGPU(const Scene& scene);

    // Getters para os dados convertidos
    const std::vector<GPUObject>& getObjects() const { return gpuObjects; }
    const std::vector<GPUMaterial>& getMaterials() const { return gpuMaterials; }
    const std::vector<GPULight>& getLights() const { return gpuLights; }
    const Camera& getCamera() const { return sceneCamera; }

private:
    void convertScene(const Scene& scene);

    // Membros para armazenar os dados prontos para a GPU
    std::vector<GPUObject>   gpuObjects;
    std::vector<GPUMaterial> gpuMaterials;
    std::vector<GPULight>    gpuLights;
    Camera                   sceneCamera;

    // Mapa para evitar duplicar materiais e rastrear índices
    std::map<std::shared_ptr<Material>, int> materialIndexMap;
    int getMaterialIndex(const std::shared_ptr<Material>& material);
};

} // namespace Prism

#endif // PRISM_VULKAN_SCENE_DATA_HPP_