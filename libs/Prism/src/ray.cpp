#include "Prism/ray.hpp"
#include "Prism/camera.hpp"
#include "Prism/matrix.hpp"
#include "Prism/objects.hpp"
#include "Prism/point.hpp"
#include "Prism/utils.hpp"
#include "Prism/vector.hpp"
#include <cmath>
#include <stdexcept>

using ld = long double;

namespace Prism {

Ray::Ray(const Point3& origin_pt, const Vector3& direction_vec) {
    Point3* newpoint = new Point3;
    Vector3* newvec = new Vector3;

    *newpoint = origin_pt;
    origin = newpoint;
    *newvec = direction_vec.normalize();
    direction = newvec;
}

Ray::Ray(const Point3& origin_pt, const Point3& target_point) {
    Point3* newpoint = new Point3;
    Vector3* newvec = new Vector3;

    *newpoint = origin_pt;
    origin = newpoint;

    *newvec = (target_point - origin_pt).normalize();

    direction = newvec;
}

Vector3* Ray::Direction() const {
    return direction;
}

HitRecord Ray::Gethit(const std::vector<Object*>& objects, const ld& t_min, const ld& t_max) {

    HitRecord first_hit;
    first_hit.t = t_max;
    for (int i = 0; i < objects.size(); i++) {
        HitRecord rec;
        bool hit_happened = (objects[i]->hit(*this, t_min, t_max, rec));

        first_hit = (hit_happened and first_hit.t > rec.t) ? rec : first_hit;
    }

    return first_hit;
}

Point3 Ray::at(const ld& t) const {
    if (origin == nullptr || direction == nullptr) {
        throw std::runtime_error("Ray origin or direction is not initialized.");
    }
    return *origin + (*direction * t);
}

} // namespace Prism