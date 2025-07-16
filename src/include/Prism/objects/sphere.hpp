/**
 * @file sphere.hpp
 * @brief Prism Render Engine
 *
 * @copyright Copyright (c) 2025 src-lua
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef PRISM_SPHERE_HPP_
#define PRISM_SPHERE_HPP_

#include "prism_export.h"

#include "Prism/core/material.hpp"
#include "Prism/core/point.hpp"
#include "Prism/core/ray.hpp"
#include "Prism/objects/objects.hpp"

namespace Prism {

/**
 * @class Sphere
 * @brief Represents a sphere in 3D space defined by its center and radius.
 * This class inherits from Object and implements the hit method to check for ray-sphere
 * intersections.
 */
class PRISM_EXPORT Sphere : public Object {
  public:
    /**
     * @brief Constructs a Sphere given its center, radius, and material.
     * @param center A Point3 representing the center of the sphere.
     * @param radius The radius of the sphere.
     * @param material A shared pointer to a Material object associated with the sphere.
     * This constructor initializes the sphere with the specified center, radius, and material
     * properties.
     */
    Sphere(Point3 center, double radius, std::shared_ptr<Material> material);

    /**
     * @brief Checks if a ray intersects with the sphere.
     * @param ray The Ray to test for intersection with the sphere.
     * @param t_min The minimum distance for a valid hit.
     * @param t_max The maximum distance for a valid hit.
     * @param rec The HitRecord to be filled upon a collision.
     * @return True if the ray intersects the sphere within the specified distance range, false
     * otherwise. This method transforms the ray using the inverse transformation matrix of the
     * sphere and checks for intersection. If a hit is found, it updates the hit record with the
     * intersection point, normal, and material properties.
     */
    virtual bool hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const override;

  private:
    Point3 center; ///< The center point of the sphere
    double radius; ///< The radius of the sphere
    std::shared_ptr<Material>
        material; ///< Material properties of the sphere, defining how it interacts with light
};

} // namespace Prism

#endif // PRISM_SPHERE_HPP_