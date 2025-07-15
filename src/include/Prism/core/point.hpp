#ifndef PRISM_POINT_HPP_
#define PRISM_POINT_HPP_

#include "prism_export.h"

#include <initializer_list>

namespace Prism {

class Vector3; // Forward declaration of Vector3 class

/**
 * @class Point3
 * @brief Represents a point in 3D space and provides common points operations.
 *
 * The Point3 class represents a point in 3D space with x, y, and z coordinates.
 */
class PRISM_EXPORT Point3 {
  public:
    /**
     * @brief Constructs a Point with given x, y, z coordinates.
     * @param x The x coordinate (default is 0).
     * @param y The y coordinate (default is 0).
     * @param z The z coordinate (default is 0).
     */
    Point3(double x = 0, double y = 0, double z = 0);

    /**
     * @brief Copy constructor.
     * @param v The point to copy from.
     */
    Point3(const Point3& p);

    /**
     * @brief Constructs a Point from a Vector3.
     * @param v The vector to convert into a point.
     */
    Point3(const Vector3& v);

    /**
     * @brief Constructs a Point from an initializer list of coordinates.
     * @param coords An initializer list containing the x, y, and z coordinates.
     */
    Point3(std::initializer_list<double> coords);

    /**
     * @brief Asignment operator.
     * @param p The point to assign from.
     * @return Reference to this point.
     */
    Point3 operator=(const Point3& p);

    /**
     * @brief Checks if two points are equal.
     * @param p The point to compare with.
     * @return True if all coordinates are equal, false otherwise.
     */
    bool operator==(const Point3& p) const;

    /**
     * @brief Negates the point.
     * @return A new point that is the negation of this point.
     */
    Point3 operator-() const;

    /**
     * @brief Gets a vector from this point to another point.
     * @param p The point to get the vector to.
     * @return The vector from this point to the specified point.
     */
    Vector3 operator-(const Point3& p) const;

    /**
     * @brief Adds a vector to this point.
     * @param v The vector to add.
     * @return A new point that is the result of adding the vector to this point.
     */
    Point3 operator+(const Vector3& v) const;

    /**
     * @brief Adds a vector to this point in place.
     * @param v The vector to add.
     * @return Reference to this point after addition.
     */
    Point3 operator+=(const Vector3& v);

    double x; ///< The x coordinate of the point.
    double y; ///< The y coordinate of the point.
    double z; ///< The z coordinate of the point.
};

} // namespace Prism

#endif // PRISM_POINT_HPP_