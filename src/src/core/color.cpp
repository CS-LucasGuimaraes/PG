#include "Prism/core/color.hpp"

namespace Prism {

Color::Color() : r(0), g(0), b(0) {
}
Color::Color(double red, double green, double blue) : r(red), g(green), b(blue) {
}
Color::Color(int red, int green, int blue)
    : r(static_cast<double>(red) / 255.0), g(static_cast<double>(green) / 255.0),
      b(static_cast<double>(blue) / 255.0) {
}
Color::Color(const Color& other) : r(other.r), g(other.g), b(other.b) {
}

} // namespace Prism