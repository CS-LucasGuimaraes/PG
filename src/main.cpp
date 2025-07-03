#include "Prism.hpp"
#include "Prism/plane.hpp"
#include <memory>
#include <vector>

int main() {
    // Configuração da Câmera
    Prism::Point3 lookfrom(-4, 4, 4);
    Prism::Point3 lookat(0, 0, -0.75);
    Prism::Vector3 vup(0, 1, 0);
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 480;
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
   
    auto cube = std::make_unique<Prism::Mesh>(reader);

    Prism::Matrix teste = Prism::Matrix(4,4);
    teste[0][0] = 1; teste[0][1] = 0; teste[0][2] = 0; teste[0][3] = 2;
    teste[1][0] = 0; teste[1][1] = 1; teste[1][2] = 0; teste[1][3] = 2;
    teste[2][0] = 0; teste[2][1] = 0; teste[2][2] = 1; teste[2][3] = 0;
    teste[3][0] = 0; teste[3][1] = 0; teste[3][2] = 0; teste[3][3] = 1;

    cube->setTransform(
        // Prism::Matrix::translation(3, {2, 2, 0}) * 
        teste *
        Prism::Matrix::rotation3d(45, {1, 0, 0}) * 
        Prism::Matrix::scaling(3, {0.5, 0.5, 0.5})
    );

    scene.addObject(move(cube));

    auto plane = std::make_unique<Prism::Plane>(
        Prism::Point3(0, -0.5, 0), Prism::Vector3(0, 1, 0), material_chao
    );

    // plane->setTransform(Prism::Matrix::rotation3d(45, {1,0,0}));
    

    scene.addObject(move(plane));

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