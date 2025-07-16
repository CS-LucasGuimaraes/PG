/**
 * @file plane.hpp
 * @brief Prism Render Engine
 *
 * @copyright Copyright (c) 2025 src-lua
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef PRISM_PLANE_HPP_
#define PRISM_PLANE_HPP_

#include "prism_export.h"

#include "Prism/core/material.hpp"
#include "Prism/core/point.hpp"
#include "Prism/core/ray.hpp"
#include "Prism/objects/objects.hpp"

namespace Prism {

/**
 * @class Plane
 * @brief Represents a plane in 3D space defined by a point and a normal vector.
 * This class inherits from Object and implements the hit method to check for ray-plane
 * intersections.
 */
class PRISM_EXPORT Plane : public Object {
  public:
    /**
     * @brief Constructs a Plane given a point on the plane, a normal vector, and a material.
     * @param point_on_plane A Point3 representing a point on the plane.
     * @param normal A Vector3 representing the normal vector of the plane.
     * @param material A shared pointer to a Material object associated with the plane.
     */
    Plane(Point3 point_on_plane, Vector3 normal, std::shared_ptr<Material> material);

    /**
     * @brief Checks if a ray intersects with the plane.
     * @param ray The Ray to test for intersection.
     * @param t_min The minimum distance for a valid hit.
     * @param t_max The maximum distance for a valid hit.
     * @param rec The HitRecord to be filled upon a collision.
     * @return True if the ray intersects the plane within the specified distance range, false
     * otherwise.
     */
    virtual bool hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const override;

  private:
    Point3 point_on_plane; ///< A point on the plane
    Vector3 normal;        ///< The normal vector of the plane
    std::shared_ptr<Material>
        material; ///< Material properties of the plane, defining how it interacts with light
};

} // namespace Prism

#endif // PRISM_PLANE_HPP_