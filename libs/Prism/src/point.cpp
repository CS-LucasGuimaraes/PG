#include "Prism/point.hpp"
#include "Prism/vector.hpp"
#include "Prism/matrix.hpp"
#include <stdexcept>

namespace Prism {

Point3::Point3(double x, double y, double z) : x(x), y(y), z(z) {
}

Point3::Point3(const Point3& p) : x(p.x), y(p.y), z(p.z) {
}

Point3::Point3(const Vector3& v) : x(v.x), y(v.y), z(v.z) {
}

Point3::Point3(std::initializer_list<double> coords) {
    if (coords.size() != 3) {
        throw std::invalid_argument("Initializer list must contain exactly three elements.");
    }
    auto it = coords.begin();
    x = *it++;
    y = *it++;
    z = *it;
}

Point3 Point3::operator=(const Point3& p) {
    if (this != &p) {
        x = p.x;
        y = p.y;
        z = p.z;
    }
    return *this;
}

bool Point3::operator==(const Point3& p) const {
    return (x == p.x && y == p.y && z == p.z);
}

Vector3 Point3::operator-(const Point3& p) const {
    return Vector3(x - p.x, y - p.y, z - p.z);
}

Point3 Point3::operator+(const Vector3& v) const {
    return Point3(x + v.x, y + v.y, z + v.z);
}

Point3 Point3::operator+=(const Vector3& v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

Point3 Point3::operator*(const Matrix& m) const {
    if (!((m.getRows() == 3 && m.getCols() == 3) || (m.getRows() == 4 && m.getCols() == 4))) {
        throw std::invalid_argument("Matrix must be 3x3 or 4x4 for point transformation.");
    }

    if (m.getRows() == 3) {
        return Point3(
            m[0][0] * x + m[0][1] * y + m[0][2] * z,
            m[1][0] * x + m[1][1] * y + m[1][2] * z,
            m[2][0] * x + m[2][1] * y + m[2][2] * z
        );
    } else { // 4x4 matrix
        double w = m[3][0] * x + m[3][1] * y + m[3][2] * z + m[3][3];
        return Point3(
            (m[0][0] * x + m[0][1] * y + m[0][2] * z + m[0][3]) / w,
            (m[1][0] * x + m[1][1] * y + m[1][2] * z + m[1][3]) / w,
            (m[2][0] * x + m[2][1] * y + m[2][2] * z + m[2][3]) / w
        );
    }
}

Point3 Point3::operator*=(const Matrix& m) {
    if (!((m.getRows() == 3 && m.getCols() == 3) || (m.getRows() == 4 && m.getCols() == 4))) {
        throw std::invalid_argument("Matrix must be 3x3 or 4x4 for point transformation.");
    }

    if (m.getRows() == 3) {
        x = m[0][0] * x + m[0][1] * y + m[0][2] * z;
        y = m[1][0] * x + m[1][1] * y + m[1][2] * z;
        z = m[2][0] * x + m[2][1] * y + m[2][2] * z;
    } else { // 4x4 matrix
        double w = m[3][0] * x + m[3][1] * y + m[3][2] * z + m[3][3];
        x = (m[0][0] * x + m[0][1] * y + m[0][2] * z + m[0][3]) / w;
        y = (m[1][0] * x + m[1][1] * y + m[1][2] * z + m[1][3]) / w;
        z = (m[2][0] * x + m[2][1] * y + m[2][2] * z + m[2][3]) / w;
    }
    return *this;
}

} // namespace Prism