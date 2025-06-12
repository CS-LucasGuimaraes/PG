#include "Prism/Plane.hpp"
#include "Prism/Ray.hpp" 
#include "Prism/utils.hpp"
#include <cmath>

namespace Prism {

Plane::Plane(Point3 point_on_plane, Vector3 normal, Material* material)
    : point_on_plane(point_on_plane), normal(normal), material(material) {}


bool Plane::hit(const Ray& ray, ld t_min, ld t_max, HitRecord& rec) const {
    return false
}

} // namespace Prism