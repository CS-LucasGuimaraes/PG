#include "Prism/ray.hpp"
#include "Prism/camera.hpp"
#include "Prism/matrix.hpp"
#include "Prism/objects.hpp"
#include "Prism/point.hpp"
#include "Prism/utils.hpp"
#include "Prism/vector.hpp"
#include <cmath>
#include <stdexcept>

namespace Prism {

Ray::Ray(const Point3& origin_pt, const Vector3& direction_vec) : origin_(origin_pt), direction_(direction_vec.normalize()) {
    if (direction().magnitude() == 0) {
        throw std::invalid_argument("Direction vector cannot be zero length.");
    }
}

Ray::Ray(const Point3& origin_pt, const Point3& target_point) : origin_(origin_pt), direction_((target_point - origin_pt).normalize()) {
    if (direction().magnitude() == 0) {
        throw std::invalid_argument("Direction vector cannot be zero length.");
    }
}

HitRecord Ray::Gethit(const std::vector<std::unique_ptr<Object>>& objects, const double& t_min,
    const double& t_max) {

    HitRecord first_hit;
    first_hit.t = t_max;
    for (int i = 0; i < objects.size(); i++) {
        HitRecord rec;
        bool hit_happened = (objects[i]->hit(*this, t_min, t_max, rec));
        
        first_hit = (hit_happened and first_hit.t > rec.t) ? rec : first_hit;
    }
    
    return first_hit;
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

} // namespace Prism