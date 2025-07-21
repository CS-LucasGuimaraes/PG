/**
 * @file objects.hpp
 * @brief Prism Render Engine
 *
 * @copyright Copyright (c) 2025 src-lua
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef PRISM_OBJECT_HPP_
#define PRISM_OBJECT_HPP_

#include "prism_export.h"

#include "Prism/core/matrix.hpp"
#include "Prism/core/point.hpp"
#include "Prism/core/ray.hpp"
#include "Prism/core/vector.hpp"
#include "Prism/objects/aabb.hpp"

#include <memory>

namespace Prism {

class Ray;      // Forward declaration of Ray class
class Material; // Forward declaration of Material class

/**
 * @class HitRecord
 * @brief Represents the details of a ray-object intersection.
 * This class stores the intersection point, normal at the intersection, distance along the ray,
 * material properties, and whether the hit is on the front face of the object.
 */
struct PRISM_EXPORT HitRecord {
    Point3 p;
    Vector3 normal;
    double t;
    std::shared_ptr<Material> material;
    bool front_face;

    inline void set_face_normal(const Ray& ray, const Vector3& outward_normal) {
        front_face = (ray.direction()).dot(outward_normal) < 0;
        normal = front_face ? outward_normal : outward_normal * -1;
    }
};

/**
 * @class Object
 * @brief Abstract base class for all objects in the scene.
 * This class defines the interface for objects that can be hit by rays, providing a method to check
 * for intersections. It also manages the transformation of the object in 3D space using a
 * transformation matrix.
 */
class PRISM_EXPORT Object {
  public:
    virtual ~Object() = default;

    /**
     * @brief Checks if a ray hits the object within a given distance range.
     * @param ray The ray to test.
     * @param t_min The minimum distance for a valid hit.
     * @param t_max The maximum distance for a valid hit.
     * @param rec The hit record to be filled upon a collision.
     * @return True if a valid hit was found, false otherwise.
     */
    virtual bool hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const = 0;

    virtual AABB get_bounding_box() const = 0;

    /**
     * @brief Gets the transformation matrix of the object.
     * @param The transformation matrix.
     * This matrix represents the object's position, orientation, and scale in the scene.
     */
    void setTransform(const Matrix& new_transform) {
        transform = new_transform;
        inverseTransform = transform.inverse();
        inverseTransposeTransform = inverseTransform.transpose();
    }

    /**
     * @brief Gets the transformation matrix of the object.
     * @return The transformation matrix.
     * This matrix can be used to transform points or vectors in the object's local space to world
     * space.
     */
    Matrix getTransform() const {
        return transform;
    }

  protected:
    Matrix transform = Matrix::identity(4);        ///< Transformation matrix for the object
    Matrix inverseTransform = Matrix::identity(4); ///< Inverse of the transformation matrix
    Matrix inverseTransposeTransform = Matrix::identity(4); ///< M^-t of the transformation matrix
};

} // namespace Prism

#endif // PRISM_OBJECT_HPP_