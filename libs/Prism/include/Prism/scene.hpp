#ifndef PRISM_SCENE_HPP_
#define PRISM_SCENE_HPP_

#include "Prism/camera.hpp"
#include "Prism/color.hpp"
#include "Prism/objects.hpp"
#include "Prism/ray.hpp"
#include "prism_export.h"
#include <filesystem>
#include <memory>
#include <ostream>
#include <vector>

namespace Prism {

PRISM_EXPORT inline int convert_color(double f) {
    return static_cast<int>(255.999 * f);
}

PRISM_EXPORT inline std::ostream& operator<<(std::ostream& os, const Color& color) {
    os << static_cast<int>(convert_color(color.r)) << " "
       << static_cast<int>(convert_color(color.g)) << " "
       << static_cast<int>(convert_color(color.b));
    return os;
}

PRISM_EXPORT std::filesystem::path generate_filename();

class PRISM_EXPORT Scene {
  public:
    /**
     * @brief Constrói a cena, movendo uma câmera para dentro dela.
     * @param camera A câmera que será usada para renderizar a cena.
     */
    Scene(Camera camera);

    /**
     * @brief Adiciona um objeto à cena.
     * A posse do objeto é transferida para a cena.
     * @param object Um unique_ptr para um objeto derivado de Object (e.g., Sphere).
     */
    void addObject(std::unique_ptr<Object> object);

    /**
     * @brief Renderiza a cena a partir da perspectiva da câmera e salva em um arquivo PPM.
     */
    void render() const;

  private:
    std::vector<std::unique_ptr<Object>> objects_;
    Camera camera_;
};
} // namespace Prism

#endif // PRISM_SCENE_HPP_