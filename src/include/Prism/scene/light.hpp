#ifndef PRISM_LIGHT_HPP_
#define PRISM_LIGHT_HPP_

#include "prism_export.h"

#include "Prism/core/color.hpp"
#include "Prism/core/point.hpp"

namespace Prism {

/**
 * @class Light
 * @brief Represents a point light source in the scene.
 */
class PRISM_EXPORT Light {
  public:
    Point3 position; ///< The position of the light in 3D space.
    Color
        color; ///< The color of the light, typically used to determine how it illuminates objects.

    /**
     * @brief Default constructor that initializes the light with default values.
     * The default values are:
     * - Position: (0, 0, 0)
     * - Color: White (1, 1, 1)
     * - Intensity: 1.0
     */
    Light(const Point3& pos, const Color& col) : position(pos), color(col) {
    }
};

} // namespace Prism

#endif // PRISM_LIGHT_HPP_