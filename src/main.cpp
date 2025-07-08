#include "Prism.hpp"
#include <iostream>

int main() {
    try {
        Prism::SceneParser("./data/input/scene.yml").parse().render();
        
    } catch (const std::exception& e) {
        Prism::Style::logError(e.what());
        return 1;
    }

    return 0;
}