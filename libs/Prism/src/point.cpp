#include "Prism/point.hpp"
#include "Prism/vector.hpp"
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

} // namespace Prism