#ifndef PRISM_MATERIAL_HPP_
#define PRISM_MATERIAL_HPP_

#include "Prism/color.hpp"
#include "Prism/vector.hpp"
#include "prism_export.h"

namespace Prism {

/**
 * @class Material
 * @brief Represents a material with various properties used in rendering.
 * The Material class encapsulates the properties of a material, including its color, ambient
 * reflectivity (ka), specular reflectivity (ks), emissive color (ke), shininess (ns), index of
 * refraction (ni), and transparency (d). These properties are commonly used in computer graphics to
 * define how a material interacts with light and how it appears in a rendered scene.
 */
class PRISM_EXPORT Material {
  public:
    /**
     * @brief
     * Default constructor that initializes the material with default values.
     * The default values are:
     * - Color: Black (0, 0, 0)
     * - Ambient reflectivity (ka): (0, 0, 0)
     * - Specular reflectivity (ks): (0, 0, 0)
     * - Emissive color (ke): (0, 0, 0)
     * - Shininess (ns): 0
     * - Index of refraction (ni): 0
     * - Transparency (d): 0
     */
    Material(Color color = Color(), Vector3 ka = Vector3(), Vector3 ks = Vector3(),
             Vector3 ke = Vector3(), double ns = 0, double ni = 0, double d = 0)
        : color(color), ka(ka), ks(ks), ke(ke), ns(ns), ni(ni), d(d) {
    }

    Color color; ///< The color of the material, typically used for diffuse reflection.
    Vector3 ka;  ///< Ambient reflectivity of the material, representing how much ambient light it
                 ///< reflects.
    Vector3 ks;  ///< Specular reflectivity of the material, representing how much specular light it
                 ///< reflects.
    Vector3
        ke; ///< Emissive color of the material, representing light emitted by the material itself.
    double ns; ///< Shininess factor of the material, affecting the size and intensity of specular
               ///< highlights.
    double ni; ///< Index of refraction of the material, used for simulating refraction effects.
    double d; ///< Transparency of the material, where 0 is fully opaque and 1 is fully transparent.
};

} // namespace Prism

#endif // PRISM_MATERIAL_HPP_