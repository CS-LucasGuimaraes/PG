#include "Prism/objects/plane.hpp"

#include "Prism/core/matrix.hpp"

#include <cmath>

namespace Prism {

Plane::Plane(Point3 point_on_plane, Vector3 normal, std::shared_ptr<Material> material)
    : point_on_plane(point_on_plane), normal(normal), material(std::move(material)) {
}

bool Plane::hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const {
    Ray transformed_ray = ray.transform(inverseTransform);

    double denominator = normal.dot(transformed_ray.direction());
    double tolerance = 1e-6;

    if (std::abs(denominator) <= tolerance) {
        return false;
    }

    Vector3 world_normal = (this->inverseTransposeTransform * this->normal).normalize();
    Vector3 world_point_on_plane = transform * point_on_plane;
    double t = (world_point_on_plane - ray.origin()).dot(world_normal) / denominator;

    if (t < t_min || t > t_max) {
        return false;
    }

    rec.t = t;
    rec.p = ray.at(t); // Ponto de volta para o espaço global
    rec.set_face_normal(ray, world_normal); // Usa o raio original
    rec.material = material;

    return true;
}

} // namespace Prism