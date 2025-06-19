#include "Prism/vector.hpp"
#include "Prism/point.hpp"
#include "Prism/matrix.hpp"

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

Vector3 Vector3::operator*(const Matrix& m) const {
    if (!((m.getRows() == 3 && m.getCols() == 3) || (m.getRows() == 4 && m.getCols() == 4))) {
        throw std::invalid_argument("Matrix must be 3x3 or 4x4 for vector transformation.");
    }

    if (m.getRows() == 3) {
        return Vector3(
            m[0][0] * x + m[0][1] * y + m[0][2] * z,
            m[1][0] * x + m[1][1] * y + m[1][2] * z,
            m[2][0] * x + m[2][1] * y + m[2][2] * z
        );
    } else { // 4x4 matrix
        double w = m[3][0] * x + m[3][1] * y + m[3][2] * z + m[3][3];
        return Vector3(
            (m[0][0] * x + m[0][1] * y + m[0][2] * z + m[0][3]) / w,
            (m[1][0] * x + m[1][1] * y + m[1][2] * z + m[1][3]) / w,
            (m[2][0] * x + m[2][1] * y + m[2][2] * z + m[2][3]) / w
        );
    }
}

Vector3 Vector3::operator*=(const Matrix& m) {
    if (!((m.getRows() == 3 && m.getCols() == 3) || (m.getRows() == 4 && m.getCols() == 4))) {
        throw std::invalid_argument("Matrix must be 3x3 or 4x4 for vector transformation.");
    }

    double newx, newy, newz;

    if (m.getRows() == 3) {
        newx = m[0][0] * x + m[0][1] * y + m[0][2] * z;
        newy = m[1][0] * x + m[1][1] * y + m[1][2] * z;
        newz = m[2][0] * x + m[2][1] * y + m[2][2] * z;
    }
    else { // 4x4 matrix
        double w = m[3][0] * x + m[3][1] * y + m[3][2] * z + m[3][3];
        newx = (m[0][0] * x + m[0][1] * y + m[0][2] * z + m[0][3]) / w;
        newy = (m[1][0] * x + m[1][1] * y + m[1][2] * z + m[1][3]) / w;
        newz = (m[2][0] * x + m[2][1] * y + m[2][2] * z + m[2][3]) / w;
    }
    x = newx;
    y = newy;
    z = newz;
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