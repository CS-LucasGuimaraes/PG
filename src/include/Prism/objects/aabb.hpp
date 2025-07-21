#ifndef PRISM_AABB_HPP_
#define PRISM_AABB_HPP_

#include "Prism/core/point.hpp"
#include "Prism/core/ray.hpp"

namespace Prism {
    
class AABB {
public:
    Point3 min;
    Point3 max;

    AABB() {}
    AABB(const Point3& a, const Point3& b) : min(a), max(b) {}

    bool hit(const Ray& r, double t_min, double t_max) const;

    bool is_finite() const;
};


} // namespace Prism

#endif // PRISM_AABB_HPP_
