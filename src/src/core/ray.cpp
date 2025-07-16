/**
 * @file ray.cpp
 * @brief Prism Render Engine
 *
 * @copyright Copyright (c) 2025 src-lua
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "Prism/core/ray.hpp"

#include <cmath>
#include <stdexcept>

namespace Prism {

Ray::Ray(const Point3& origin_pt, const Vector3& direction_vec)
    : origin_(origin_pt), direction_(direction_vec.normalize()) {
    if (direction().magnitude() == 0) {
        throw std::invalid_argument("Direction vector cannot be zero length.");
    }
}

Ray::Ray(const Point3& origin_pt, const Point3& target_point)
    : origin_(origin_pt), direction_((target_point - origin_pt).normalize()) {
    if (direction().magnitude() == 0) {
        throw std::invalid_argument("Direction vector cannot be zero length.");
    }
}

Vector3 Ray::direction() const {
    return direction_;
}

Point3 Ray::origin() const {
    return origin_;
}

Point3 Ray::at(const double& t) const {
    return origin_ + (direction_ * t);
}

Ray Ray::transform(const Matrix& m) const {
    return Ray(m * origin_, m * direction_);
}

} // namespace Prism