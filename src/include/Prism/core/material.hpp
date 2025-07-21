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
#include "Prism/core/texture.hpp"

#include <memory> 

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
    Material() {
        texture = std::make_shared<SolidColor>(1.0, 1.0, 1.0);
    }
    Material(Color c) : texture(std::make_shared<SolidColor>(c)) {
    };

    Material(Color c, Color ka, Color ks, Color ke, double ns, double ni, double d) :
        texture(std::make_shared<SolidColor>(c)),
        ka(ka), ks(ks), ke(ke), ns(ns), ni(ni), d(d) {}
        
    Material(std::shared_ptr<Texture> tex, Color ka, Color ks, Color ke, double ns, double ni, double d) :
        texture(tex), ka(ka), ks(ks), ke(ke), ns(ns), ni(ni), d(d) {}

    std::shared_ptr<Texture> texture;

    Color ka = Color(0.1, 0.1, 0.1);    ///< Ambient reflectivity
    Color ks = Color(0, 0, 0);    ///< Specular reflectivity
    Color ke = Color(0, 0, 0);   ///< Emissive color
    double ns = 1.0; ///< Shininess factor
    double ni = 1.0; ///< Index of refraction
    double d = 1.0; ///< Opacity
};

} // namespace Prism

#endif // PRISM_MATERIAL_HPP_