#include "Prism/Plane.hpp"
#include "Prism/Ray.hpp" 
#include "Prism/utils.hpp"
#include <cmath>

namespace Prism {

Plane::Plane(Point3 point_on_plane, Vector3 normal, Material* material)
    : point_on_plane(point_on_plane), normal(normal), material(material) {}


bool Plane::hit(const Ray& ray, ld t_min, ld t_max, HitRecord& rec) const {
    // Calculate the denominator
    ld denominator = normal.dot(ray.direction);
    
    // If the denominator is zero, the ray is parallel to the plane
    if (std::abs(denominator) == 0) {
        return false; // No intersection
    }

    // Calculate the distance from the ray origin to the plane
    ld t = (point_on_plane - ray.origin).dot(normal) / denominator;

    // Check if the intersection point is within the bounds
    if (t < t_min || t > t_max) {
        return false; // Intersection outside of bounds
    }

    // Populate the hit record
    rec.t = t;
    rec.point = ray.at(t);
    rec.normal = normal;
    rec.material = material;

    return true; // Intersection occurred
}

} // namespace Prism