/**
 * @file color.hpp
 * @brief Prism Render Engine
 *
 * @copyright Copyright (c) 2025 src-lua
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef PRISM_COLOR_HPP_
#define PRISM_COLOR_HPP_

#include "prism_export.h"

namespace Prism {
/**
 * @class Color
 * @brief Represents a color in RGB format.
 *  The Color class encapsulates a color defined by its red, green, and blue components.
 *  Each component is a double value ranging from 0.0 to 1.0, representing the intensity of the
 * color. The class provides constructors for initializing colors with double or integer values.
 */
class PRISM_EXPORT Color {
  public:
    /**
     * @brief Default constructor that initializes the color to black (0, 0, 0).
     */
    Color();

    /**
     * @brief Constructor that initializes the color with specified red, green, and blue values.
     * @param red The red component of the color (0.0 to 1.0).
     * @param green The green component of the color (0.0 to 1.0).
     * @param blue The blue component of the color (0.0 to 1.0).
     */
    Color(double red, double green, double blue);

    /**
     * @brief Constructor that initializes the color with specified red, green, and blue values as
     * integers.
     * @param red The red component of the color (0 to 255).
     * @param green The green component of the color (0 to 255).
     * @param blue The blue component of the color (0 to 255).
     * This constructor converts the integer values to double in the range of 0.0 to 1.0.
     */
    Color(int red, int green, int blue);

    /**
     * @brief Copy constructor that initializes the color from another Color object.
     * @param other The Color object to copy from.
     */
    Color(const Color& other);

    Color operator*(Color other) const;

    Color operator*(double scalar) const;

    Color operator+(Color other) const;

    Color operator+=(Color other);

    Color& clamp();

    double r; ///< Red component of the color (0.0 to 1.0)
    double g; ///< Green component of the color (0.0 to 1.0)
    double b; ///< Blue component of the color (0.0 to 1.0)
};

} // namespace Prism
#endif // PRISM_COLOR_HPP_