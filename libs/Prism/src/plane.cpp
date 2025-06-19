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
    Ray transformed_ray = ray.transform(inverseTransform);

    double denominator = normal.dot(transformed_ray.direction());
    double tolerance = 1e-6;
    
    if (std::abs(denominator) <= tolerance) {
        return false;
    }

    double t = (point_on_plane - transformed_ray.origin()).dot(normal) / denominator;

    if (t < t_min || t > t_max) {
        return false;
    }

    rec.t = t;
    rec.p = transform * transformed_ray.at(t); // Ponto de volta para o espaço global
    Vector3 outward_normal_world = (inverseTransposeTransform * this->normal).normalize();
    rec.set_face_normal(ray, outward_normal_world); // Usa o raio original
    rec.material = material;
    
    return true;
}

} // namespace Prism