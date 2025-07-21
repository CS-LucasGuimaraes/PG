#include "Prism/scene/acceleration.hpp"

namespace Prism {

NoAcceleration::NoAcceleration(std::vector<Object*>& objects) : objects_(objects) {
}

bool NoAcceleration::hit_closest(const Ray& ray, double t_min, double t_max, HitRecord& rec) const {
    bool hit_anything = false;
    double closest_t = INFINITY;

    for (const auto& object_ptr : objects_) {
        HitRecord temp_rec;
        if (object_ptr->hit(ray, 1e-4, closest_t, temp_rec)) {
            hit_anything = true;
            closest_t = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

bool NoAcceleration::hit_any(const Ray& ray, double t_min, double t_max, HitRecord& rec) const {
    for (const auto& object_ptr : objects_) {
        HitRecord temp_rec;
        if (object_ptr->hit(ray, t_min, t_max, temp_rec)) {
            rec = temp_rec;
            return true;
        }
    }
    return false;
}

} // namespace Prism
