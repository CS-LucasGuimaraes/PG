#include "Prism/plane.hpp"
#include "Prism/ray.hpp" 
#include "Prism/utils.hpp"
#include "Prism/point.hpp"
#include "Prism/vector.hpp"
#include "Prism/material.hpp"
#include <cmath>

namespace Prism {

Plane::Plane(Point3 point_on_plane, Vector3 normal, std::shared_ptr<Material>material)
    : point_on_plane(point_on_plane), normal(normal), material(std::move(material)) {}


bool Plane::hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const {
    // Calculate the denominator
    float denominator = normal.dot(ray.direction());
    float tolerance = 1e-6; // A small value to avoid floating-point precision issues
    
    // If the denominator is zero, the ray is parallel to the plane
    if (std::abs(denominator) <= tolerance) {
        return false; // No intersection
    }

    // Calculate the distance from the ray origin to the plane
    float t = (point_on_plane - ray.origin()).dot(normal) / denominator;

    // Check if the intersection point is within the bounds
    if (t < t_min || t > t_max) {
        return false; // Intersection outside of bounds
    }

    // Populate the hit record
    rec.t = t;
    rec.p = ray.at(t);
    rec.set_face_normal(ray, this->normal);
    rec.material = material;
    

    return true; // Intersection occurred
}

} // namespace Prism