#include "Prism/vulkan/vulkan_scene_data.hpp"
#include "Prism/objects/plane.hpp"
#include "Prism/objects/sphere.hpp"
// Inclua outros tipos de objetos que você queira suportar, como Mesh.
// #include "Prism/objects/mesh.hpp"

#include <cstring> // Para memcpy
#include <stdexcept>

namespace Prism {

SceneDataGPU::SceneDataGPU(const Scene& scene) : sceneCamera(scene.getCamera()) {
    convertScene(scene);
}

int SceneDataGPU::getMaterialIndex(const std::shared_ptr<Material>& material) {
    // Se já processamos este material, retorna o seu índice
    if (materialIndexMap.count(material)) {
        return materialIndexMap[material];
    }

    // Se for um novo material, converte-o e armazena-o
    GPUMaterial gpuMat = {};
    gpuMat.color[0] = material->color.r;
    gpuMat.color[1] = material->color.g;
    gpuMat.color[2] = material->color.b;
    gpuMat.refraction_index = material->ni;

    gpuMat.ambient[0] = material->ka.r;
    gpuMat.ambient[1] = material->ka.g;
    gpuMat.ambient[2] = material->ka.b;
    gpuMat.transparency = material->d;

    gpuMat.specular[0] = material->ks.r;
    gpuMat.specular[1] = material->ks.g;
    gpuMat.specular[2] = material->ks.b;
    gpuMat.shininess = material->ns;

    gpuMat.emission[0] = material->ke.r;
    gpuMat.emission[1] = material->ke.g;
    gpuMat.emission[2] = material->ke.b;

    int newIndex = gpuMaterials.size();
    gpuMaterials.push_back(gpuMat);
    materialIndexMap[material] = newIndex;

    return newIndex;
}

void SceneDataGPU::convertScene(const Scene& scene) {
    // Converte as luzes
    for (const auto& light : scene.getLights()) {
        GPULight gpuLight = {};
        gpuLight.position[0] = light->position.x;
        gpuLight.position[1] = light->position.y;
        gpuLight.position[2] = light->position.z;

        gpuLight.color[0] = light->color.r;
        gpuLight.color[1] = light->color.g;
        gpuLight.color[2] = light->color.b;
        gpuLights.push_back(gpuLight);
    }

    // Converte os objetos
    for (const auto& object : scene.getObjects()) {
        GPUObject gpuObj = {};

        // --- Identifica o tipo de objeto ---
        // Usamos dynamic_cast para descobrir o tipo real do objeto.

        if (auto sphere = dynamic_cast<const Sphere*>(object.get())) {
            gpuObj.type = 0; // 0 = Esfera
            gpuObj.center[0] = sphere->getCenter().x;
            gpuObj.center[1] = sphere->getCenter().y;
            gpuObj.center[2] = sphere->getCenter().z;
            gpuObj.radius = sphere->getRadius();
            gpuObj.material_index = getMaterialIndex(sphere->getMaterial());

        } else if (auto plane = dynamic_cast<const Plane*>(object.get())) {
            gpuObj.type = 1; // 1 = Plano
            gpuObj.point_on_plane[0] = plane->getPointOnPlane().x;
            gpuObj.point_on_plane[1] = plane->getPointOnPlane().y;
            gpuObj.point_on_plane[2] = plane->getPointOnPlane().z;

            gpuObj.normal[0] = plane->getNormal().x;
            gpuObj.normal[1] = plane->getNormal().y;
            gpuObj.normal[2] = plane->getNormal().z;
            gpuObj.material_index = getMaterialIndex(plane->getMaterial());

            // } else if (auto mesh = dynamic_cast<const Mesh*>(object.get())) {
            //     // A conversão de meshes é mais complexa e exigiria
            //     // expandir os triângulos em um único buffer.
            //     // Deixado como um próximo passo.

        } else {
            // Tipo de objeto não suportado pela GPU, ignora.
            continue;
        }

        // Copia as matrizes de transformação
        Matrix transform = object->getTransform();
        Matrix invTransform = object->getInverseTransform();
        Matrix invTTransform = object->getInverseTransposeTransform();

        // Função auxiliar para copiar e transpor
        auto copyAndTranspose = [](float* dest, const Matrix& src) {
            for (int col = 0; col < 4; ++col) {
                for (int row = 0; row < 4; ++row) {
                    // Converte de double para float e transpõe (acessa src[row][col] e escreve em
                    // dest[col][row])
                    dest[col * 4 + row] = static_cast<float>(src[row][col]);
                }
            }
        };

        copyAndTranspose(gpuObj.transform, transform);
        copyAndTranspose(gpuObj.inverse_transform, invTransform);
        copyAndTranspose(gpuObj.inverse_transpose_transform, invTTransform);

        gpuObjects.push_back(gpuObj);
    }
}

} // namespace Prism