#include "Prism/color.hpp"

namespace Prism {

Color::Color() : r(0), g(0), b(0) {}
Color::Color(float red, float green, float blue) : r(red), g(green), b(blue) {}
// Color::Color(int red, int green, int blue): r(static_cast<float>(red) / 255.0f), g(static_cast<float>(green) / 255.0f), b(static_cast<float>(blue) / 255.0f) {}
Color::Color(const Color& other) : r(other.r), g(other.g), b(other.b) {}

} // namespace Prism