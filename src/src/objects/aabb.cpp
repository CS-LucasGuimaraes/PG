#include "Prism/objects/aabb.hpp"

namespace Prism {

bool AABB::hit(const Ray& r, double t_min, double t_max) const {
    // X axis
    double invDx = 1.0 / r.direction().x;
    double tx0 = (min.x - r.origin().x) * invDx;
    double tx1 = (max.x - r.origin().x) * invDx;
    if (invDx < 0.0) std::swap(tx0, tx1);
    t_min = std::max(tx0, t_min);
    t_max = std::min(tx1, t_max);
    if (t_max <= t_min) {
        return false;
    }

    // Y axis
    double invDy = 1.0 / r.direction().y;
    double ty0 = (min.y - r.origin().y) * invDy;
    double ty1 = (max.y - r.origin().y) * invDy;
    if (invDy < 0.0) std::swap(ty0, ty1);
    t_min = std::max(ty0, t_min);
    t_max = std::min(ty1, t_max);
    if (t_max <= t_min) {
        return false;
    }

    // Z axis
    double invDz = 1.0 / r.direction().z;
    double tz0 = (min.z - r.origin().z) * invDz;
    double tz1 = (max.z - r.origin().z) * invDz;
    if (invDz < 0.0) std::swap(tz0, tz1);
    t_min = std::max(tz0, t_min);
    t_max = std::min(tz1, t_max);
    if (t_max <= t_min) {
        return false;
    }

    return true;
}

bool AABB::is_finite() const {
    return std::isfinite(min.x) && std::isfinite(min.y) && std::isfinite(min.z) &&
           std::isfinite(max.x) && std::isfinite(max.y) && std::isfinite(max.z);
}

} // namespace Prism