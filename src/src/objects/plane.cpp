/**
 * @file plane.cpp
 * @brief Prism Render Engine
 *
 * @copyright Copyright (c) 2025 src-lua
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

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
    rec.p = ray.at(t);                      // Ponto de volta para o espaço global
    rec.set_face_normal(ray, world_normal); // Usa o raio original
    rec.material = material;

    return true;
}

AABB Plane::get_bounding_box() const {
    const double inf = std::numeric_limits<double>::infinity();

    Vector3 world_normal = (inverseTransposeTransform * normal).normalize();
    Point3 world_point = transform * point_on_plane;

    const double epsilon = 1e-6;

    // --- Special case: Axis Aligned Plane ---

    // Aligned with the X axis
    if (std::abs(world_normal.x) == 1.0 && world_normal.y == 0.0 && world_normal.z == 0.0) {
        return AABB(Point3(world_point.x - epsilon, -inf, -inf),
                    Point3(world_point.x + epsilon,  inf,  inf));
    }
    // Aligned with the Y axis
    if (world_normal.x == 0.0 && std::abs(world_normal.y) == 1.0 && world_normal.z == 0.0) {
        return AABB(Point3(-inf, world_point.y - epsilon, -inf),
                    Point3( inf, world_point.y + epsilon,  inf));
    }
    // Aligned with the Z axis
    if (world_normal.x == 0.0 && world_normal.y == 0.0 && std::abs(world_normal.z) == 1.0) {
        return AABB(Point3(-inf, -inf, world_point.z - epsilon),
                    Point3( inf,  inf, world_point.z + epsilon));
    }

    // --- General Case: Rotated Planes ---
    // If the plane is not perfectly aligned, any AABB that contains it
    // needs to be infinite in all three directions. Trying to calculate a
    // "tight" bound is impossible.
    return AABB(Point3(-inf, -inf, -inf),
                Point3( inf,  inf,  inf));
}

} // namespace Prism