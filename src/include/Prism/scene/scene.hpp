#ifndef PRISM_SCENE_HPP_
#define PRISM_SCENE_HPP_

#include "prism_export.h"

#include "Prism/objects/objects.hpp"
#include "Prism/scene/camera.hpp"

#include <filesystem>
#include <memory>
#include <ostream>
#include <vector>

namespace Prism {

PRISM_EXPORT std::filesystem::path generate_filename();

/**
 * @class Scene
 * @brief Represents a 3D scene containing objects and a camera for rendering.
 * This class manages the collection of objects in the scene and provides functionality to render
 * the scene from the camera's perspective.
 */
class PRISM_EXPORT Scene {
  public:
    /**
     * @brief Constructs a Scene with a specified camera.
     * @param camera The Camera object that defines the viewpoint and projection parameters for
     * rendering the scene.
     */
    explicit Scene(Camera camera);

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

    Scene(Scene&&) = default;
    Scene& operator=(Scene&&) = default;

    /**
     * @brief Adds an object to the scene.
     * @param object A unique pointer to an Object that will be added to the scene.
     * This method takes ownership of the object and stores it in the scene's collection.
     */
    void addObject(std::unique_ptr<Object> object);

    /**
     * @brief Renders the scene from the camera's perspective.
     * This method iterates over all objects in the scene, checks for ray-object intersections, and
     * generates the final image. The rendered image is saved to a file with a timestamped filename.
     * The rendering process involves casting rays from the camera through each pixel of the
     * viewport and checking for hits with the objects.
     */
    void render() const;

  private:
    std::vector<std::unique_ptr<Object>> objects_; ///< Collection of objects in the scene
    Camera camera_;                                ///< The camera used to view the scene
};
} // namespace Prism

#endif // PRISM_SCENE_HPP_