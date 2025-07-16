#include "Prism/core/vector.hpp"

#include "Prism/core/point.hpp"

#include <cmath>
#include <stdexcept>

namespace Prism {

Vector3::Vector3(double x, double y, double z) : x(x), y(y), z(z) {
}

Vector3::Vector3(const Vector3& v) : x(v.x), y(v.y), z(v.z) {
}

Vector3::Vector3(const Point3& v) : x(v.x), y(v.y), z(v.z) {
}

Vector3::Vector3(std::initializer_list<double> coords) {
    if (coords.size() != 3) {
        throw std::invalid_argument("Initializer list must contain exactly three elements.");
    }
    auto it = coords.begin();
    x = *it++;
    y = *it++;
    z = *it;
}

bool Vector3::operator==(const Vector3& v) const {
    return (x == v.x && y == v.y && z == v.z);
}

bool Vector3::operator!=(const Vector3& v) const {
    return !(*this == v);
}

Vector3& Vector3::operator=(const Vector3& v) {
    if (this != &v) {
        x = v.x;
        y = v.y;
        z = v.z;
    }
    return *this;
}

Vector3 Vector3::operator-() const {
    return Vector3(-x, -y, -z);
}

Vector3 Vector3::operator+(const Vector3& v) const {
    return Vector3(x + v.x, y + v.y, z + v.z);
}

Vector3 Vector3::operator+=(const Vector3& v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

Vector3 Vector3::operator+(const double scalar) const {
    return Vector3(x + scalar, y + scalar, z + scalar);
}

Vector3 Vector3::operator+=(const double scalar) {
    x += scalar;
    y += scalar;
    z += scalar;
    return *this;
}

Vector3 Vector3::operator-(const Vector3& v) const {
    return Vector3(x - v.x, y - v.y, z - v.z);
}

Vector3 Vector3::operator-=(const Vector3& v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

Vector3 Vector3::operator-(const double scalar) const {
    return Vector3(x - scalar, y - scalar, z - scalar);
}

Vector3 Vector3::operator-=(const double scalar) {
    x -= scalar;
    y -= scalar;
    z -= scalar;
    return *this;
}

Vector3 Vector3::operator*(double scalar) const {
    return Vector3(x * scalar, y * scalar, z * scalar);
}

Vector3 Vector3::operator*=(double scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

Vector3 Vector3::operator/(double scalar) const {
    if (scalar == 0) {
        throw std::invalid_argument("Division by zero");
    }
    return Vector3(x / scalar, y / scalar, z / scalar);
}

Vector3 Vector3::operator/=(double scalar) {
    if (scalar == 0) {
        throw std::invalid_argument("Division by zero");
    }
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
}

double Vector3::dot(const Vector3& v) const {
    return x * v.x + y * v.y + z * v.z;
}

double Vector3::operator*(const Vector3& v) const {
    return dot(v);
}

Vector3 Vector3::cross(const Vector3& v) const {
    return Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

Vector3 Vector3::operator^(const Vector3& v) const {
    return cross(v);
}

double Vector3::magnitude() const {
    return std::sqrt(dot(*this));
}

Vector3 Vector3::normalize() const {
    double mag = magnitude();
    if (mag == 0) {
        throw std::invalid_argument("Cannot normalize a zero Vector");
    }
    return *this / mag;
}

} // namespace Prism