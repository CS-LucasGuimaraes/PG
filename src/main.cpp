#include "Prism.hpp"
#include "Prism/plane.hpp"
#include <memory>
#include <vector>

int main() {
    // Configuração da Câmera
    Prism::Point3 lookfrom(-2, 2, 2);
    Prism::Point3 lookat(0, 0, -0.75);
    Prism::Vector3 vup(0, 1, 0);
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 1920;
    int image_height = static_cast<int>(image_width / aspect_ratio);

    Prism::Camera cam(lookfrom, lookat, vup, 2.0, 2.0, 2.0 * aspect_ratio, image_height, image_width);

    // Criação da Cena
    Prism::Scene scene(std::move(cam));

    //Objreader
    Prism :: ObjReader reader("./data/input/cubo.obj");

    // Criação dos Materiais
    auto material_chao = std::make_shared<Prism::Material>(Prism::Color(0.8, 0.8, 0.8));
    auto material_esfera_1 = std::make_shared<Prism::Material>(Prism::Color(1.0, 0.3, 0.3)); // Vermelho/Rosa
    auto material_esfera_2 = std::make_shared<Prism::Material>(Prism::Color(0.3, 0.3, 1.0));   // Azul

    // Adição dos Objetos à Cena
    std::vector<std::shared_ptr<Prism::Point3>> points;
    for(auto& point: reader.vertices){
        points.emplace_back(std::make_shared<Prism::Point3>(point[0],point[1],point[2]));
    }

    for(auto& triangle: reader.triangles){
        scene.addObject(std::make_unique<Prism::Triangle<std::shared_ptr<Prism::Point3>>>(points[triangle[0]],points[triangle[1]],points[triangle[2]],material_esfera_1));
    }

    scene.addObject(std::make_unique<Prism::Plane>(
        Prism::Point3(0, -0.5, 0), Prism::Vector3(0, 1, 0), material_chao
    ));

    // scene.addObject(std::make_unique<Prism::Sphere>(
    //     Prism::Point3(0, 0, -1), 0.5, material_esfera_1
    // ));

    // scene.addObject(std::make_unique<Prism::Sphere>(
    //     Prism::Point3(-0.35, -0.2, -0.6), 0.3, material_esfera_2
    // ));
    
    // 5. Renderização
    scene.render();

    return 0;
}