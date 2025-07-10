#ifndef PRISM_RAY_HPP_
#define PRISM_RAY_HPP_

#include "prism_export.h"

#include "Prism/core/matrix.hpp"
#include "Prism/core/point.hpp"
#include "Prism/core/vector.hpp"

#include <initializer_list>
#include <memory>
#include <vector>

namespace Prism {

/**
 * @class Ray
 * @brief Represents a Ray with a direction originated from a point
 */
class PRISM_EXPORT Ray {
  public:
    /**
     * @brief Constructs a Ray given its origin and the direction at which it points
     * @param origin Point in 3d space that originates the ray.
     * @param direction normalized vector representing the direction which the ray points towards.
     */

    Ray(const Point3& origin, const Vector3& direction);
    /**
     * @brief Constucts a ray that goes from its origin torwards another given point
     * @param origin Point in 3d space that originates the ray.
     * @param target Point which the ray targets
     */
    Ray(const Point3& origin, const Point3& target);

    /**
     * @brief gets the direction of ray. if not instantiated, returns null
     */
    Vector3 direction() const;

    /**
     * @brief Transforms the ray using a transformation matrix
     * @param m The transformation matrix to apply to the ray
     * @return A new Ray object that is transformed by the given matrix
     */
    Ray transform(const Matrix& m) const;

    /**
     * @brief Gets the origin point of the ray
     * @return The origin point of the ray
     */
    Point3 origin() const;

    /**
     * @brief Gets the point at a specific distance along the ray
     * @param t The distance along the ray from its origin
     * @return The Point3 at the specified distance along the ray
     */
    Point3 at(const double& t) const;

  private:
    Point3 origin_;     ///< The origin point of the ray
    Vector3 direction_; ///< The direction vector of the ray, normalized to unit length
};

} // namespace Prism

#endif // PRISM_RAY_HPP_