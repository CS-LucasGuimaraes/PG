#ifndef PRISM_RAY_HPP_
#define PRISM_RAY_HPP_

#include "prism_export.h"
#include "Prism/point.hpp"
#include "Prism/vector.hpp"
#include <initializer_list>
#include <vector>
#include <memory>
namespace Prism {

class Object;                       // Forward declaration of Object class
struct HitRecord;                   // Forward declaration of HitRecord struct

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

    Ray transform(const Matrix& m) const;
    
    Point3 origin() const;

    Point3 at(const double& t) const;

  private:
    Point3 origin_;
    Vector3 direction_;
};

} // namespace Prism

#endif // PRISM_RAY_HPP_