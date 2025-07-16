/**
 * @file utils.cpp
 * @brief Prism Render Engine
 *
 * @copyright Copyright (c) 2025 src-lua
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

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

Vector3 refract(const Vector3& uv, const Vector3& n, double etai_over_etat) {
    auto cos_theta = std::fmin((-uv).dot(n), 1.0);
    Vector3 r_out_perp = (uv + n * cos_theta) * etai_over_etat;
    Vector3 r_out_parallel = n * -sqrt(fabs(1.0 - sqr(r_out_perp.magnitude())));

    if (sqr(r_out_perp.magnitude()) > 1.0) {
        return Vector3(0, 0, 0);
    }

    return r_out_perp + r_out_parallel;
}

double schlick(double cos, double ref_idx) {
    auto r0 = (1.0 - ref_idx) / (1.0 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1.0 - r0) * pow((1.0 - cos), 5);
}

} // namespace Prism