/**
 * @file material.hpp
 * @brief Prism Render Engine
 *
 * @copyright Copyright (c) 2025 src-lua
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef PRISM_MATERIAL_HPP_
#define PRISM_MATERIAL_HPP_

#include "prism_export.h"

#include "Prism/core/color.hpp"
#include "Prism/core/vector.hpp"

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
     * - Color: white (1,1,1)
     * - Ambient reflectivity (ka): (0.1, 0.1, 0.1)
     * - Specular reflectivity (ks): (0, 0, 0)
     * - Emissive color (ke): (0, 0, 0)
     * - Shininess (ns): 1
     * - Index of refraction (ni): 1
     * - Transparency (d): 1
     */
    Material(Color color = Color(1, 1, 1), Color ka = Color(0.1, 0.1, 0.1),
             Color ks = Color(0, 0, 0), Color ke = Color(0, 0, 0), double ns = 1.0, double ni = 1.0,
             double d = 1.0)
        : color(color), ka(ka), ks(ks), ke(ke), ns(ns), ni(ni), d(d) {
    }

    Color color; ///< The color of the material, typically used for diffuse reflection.
    Color ka;    ///< Ambient reflectivity of the material, representing how much ambient light it
                 ///< reflects.
    Color ks;    ///< Specular reflectivity of the material, representing how much specular light it
                 ///< reflects.
    Color
        ke; ///< Emissive color of the material, representing light emitted by the material itself.
    double ns; ///< Shininess factor of the material, affecting the size and intensity of specular
               ///< highlights.
    double ni; ///< Index of refraction of the material, used for simulating refraction effects.
    double d; ///< Transparency of the material, where 0 is fully opaque and 1 is fully transparent.
};

} // namespace Prism

#endif // PRISM_MATERIAL_HPP_