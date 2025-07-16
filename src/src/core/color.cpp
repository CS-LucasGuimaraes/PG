#include "Prism/core/color.hpp"

#include <algorithm>

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

Color Color::operator*(Color other) const {
    return Color(r * other.r, g * other.g, b * other.b);
}

Color Color::operator*(double scalar) const {
    return Color(r * scalar, g * scalar, b * scalar);
}

Color Color::operator+(Color other) const {
    return Color(r + other.r, g + other.g, b + other.b);
}

Color Color::operator+=(Color other) {
    r += other.r;
    g += other.g;
    b += other.b;
    return *this;
}

Color& Color::clamp() {
    r = std::max(0.0, std::min(r, 1.0));
    g = std::max(0.0, std::min(g, 1.0));
    b = std::max(0.0, std::min(b, 1.0));

    return *this;
}

} // namespace Prism