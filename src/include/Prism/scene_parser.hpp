#ifndef PRISM_SCENEPARSER_HPP_
#define PRISM_SCENEPARSER_HPP_

#include "Prism/scene.hpp"
#include "prism_export.h"
#include <string>

namespace Prism {

/**
 * @class SceneParser
 * @brief Reads and parses a scene description from a YAML file.
 * The SceneParser class is responsible for reading a YAML file that describes a 3D scene, including
 * camera settings, materials, and objects. It constructs a Scene object that can be rendered.
 */
class PRISM_EXPORT SceneParser {
  public:
    /**
     * @brief Constructor that initializes the SceneParser with the path to the scene file.
     * @param sceneFilePath The path to the YAML file containing the scene description.
     */
    explicit SceneParser(const std::string& sceneFilePath);

    /**
     * @brief Parses the scene file and constructs a Scene object.
     * @return A Scene object containing the parsed camera, materials, and objects.
     * @throws std::runtime_error if there is an error reading or parsing the YAML file.
     */
    Scene parse();

  private:
    std::string filePath; ///< The path to the YAML file containing the scene description
};

} // namespace Prism

#endif // PRISM_SCENEPARSER_HPP_