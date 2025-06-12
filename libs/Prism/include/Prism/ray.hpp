#ifndef PRISM_RAY_HPP_
#define PRISM_RAY_HPP_

#include "prism_export.h"
#include <initializer_list>
#include <vector>
namespace Prism {

using ld = long double;

class Vector3;                      // Forward declaration of Vector3 class
class Point3;                       // Forward declaration of Point3 class
template <typename T> class Matrix; // Forward declaration of Matrix class
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
     * @brief Casts rays and verifies first intersection with object
     * @param objects vector of objects within the scene
     * @param t_max maximum distance at which intersections are verified
     */
    HitRecord Gethit(const std ::vector<Object*>& objects, const ld& t_min, const ld& t_max);
    /**
     * @brief gets the direction of ray. if not instantiated, returns null
     */
    Vector3* Direction() const;

    Point3 at(const ld& t) const; 

    Point3* origin;
    Vector3* direction;
};

} // namespace Prism

#endif // PRISM_RAY_HPP_