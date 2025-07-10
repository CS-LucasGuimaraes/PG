#include "Prism/core/utils.hpp"

#include <cstddef>
#include <stdexcept>

namespace Prism {

Point3 centroid(const std::initializer_list<Point3>& points) {
    double sumX = 0, sumY = 0, sumZ = 0;
    size_t count = points.size();

    if (count == 0) {
        throw std::invalid_argument("Cannot compute centroid of an empty list of points.");
    }

    for (const auto& point : points) {
        sumX += point.x;
        sumY += point.y;
        sumZ += point.z;
    }

    return Point3(sumX / count, sumY / count, sumZ / count);
}

Matrix orthonormalBasisContaining(const Vector3& v) {

    // Step 1: Normalize v
    Vector3 v1 = v.normalize();

    // Step 2: Choose an arbitrary vector not parallel to v1
    Vector3 temp = std::abs(v1.x) < 0.9 ? Vector3{1, 0, 0} : Vector3{0, 1, 0};

    // Step 3: Orthonormalize: v2 = (temp - proj_v1(temp)).normalize()
    Vector3 proj = v1 * (v1.dot(temp));             // projection of temp onto v1
    Vector3 orthogonal = (temp - proj).normalize(); // v2

    // Step 4: v3 = v1 × v2
    Vector3 v3 = v1.cross(orthogonal);

    // Step 5: Construct 3x3 matrix with v1, v2, v3 as columns
    Matrix basis(3, 3);
    for (int i = 0; i < 3; ++i) {
        basis[i][0] = (i == 0 ? v1.x : (i == 1 ? v1.y : v1.z));
        basis[i][1] = (i == 0 ? orthogonal.x : (i == 1 ? orthogonal.y : orthogonal.z));
        basis[i][2] = (i == 0 ? v3.x : (i == 1 ? v3.y : v3.z));
    }

    return basis;
}

} // namespace Prism