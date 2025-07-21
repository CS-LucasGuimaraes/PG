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

    double t_local = (point_on_plane - transformed_ray.origin()).dot(normal) / denominator;
    Point3 world_hit_point = transform * transformed_ray.at(t_local);

    double t_global = (world_hit_point - ray.origin()).dot(ray.direction().normalize());
    
    if (t_global < t_min || t_global > t_max) {
        return false;
    }

    
    rec.t = t_global;
    rec.p = world_hit_point;
    Vector3 world_normal = (inverseTransposeTransform * normal).normalize();
    rec.set_face_normal(ray, world_normal); // Usa o raio original
    rec.material = material;

    Vector3 up_reference(0.0, 1.0, 0.0);
    if (std::abs(normal.dot(up_reference)) > 0.999) {
        up_reference = Vector3(1.0, 0.0, 0.0);
    }

    Vector3 u_dir = up_reference.cross(normal).normalize();
    Vector3 v_dir = normal.cross(u_dir);

    Point3 local_hit_point = transformed_ray.at(t_local);
    Vector3 hit_vec = local_hit_point - point_on_plane;
    rec.u = hit_vec.dot(u_dir);
    rec.v = hit_vec.dot(v_dir);

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