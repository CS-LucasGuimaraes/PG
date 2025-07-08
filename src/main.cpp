#include "Prism.hpp"
#include <iostream>

int main() {
    try {
        Prism::SceneParser("./data/input/scene.yml").parse().render();
        
    } catch (const std::exception& e) {
        std::cerr << "Ocorreu um erro: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}