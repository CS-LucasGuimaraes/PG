/**
 * @file vector.hpp
 * @brief Prism Render Engine
 *
 * @copyright Copyright (c) 2025 src-lua
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef PRISM_VECTOR_HPP_
#define PRISM_VECTOR_HPP_

#include "prism_export.h"

#include <initializer_list>

namespace Prism {

class Point3; // Forward declaration of Point3 class

/**
 * @class Vector3
 * @brief Represents a 3-dimensional mathematical vector and provides common vector operations.
 *
 * The Vector class supports arithmetic operations, dot and cross products, normalization,
 * and comparison operators. All operations are performed in 3D space.
 */
class PRISM_EXPORT Vector3 {
  public:
    /**
     * @brief Constructs a Vector with given x, y, z components.
     * @param x The x component (default is 0).
     * @param y The y component (default is 0).
     * @param z The z component (default is 0).
     */
    Vector3(double x = 0, double y = 0, double z = 0);

    /**
     * @brief Copy constructor.
     * @param v The vector to copy from.
     */
    Vector3(const Vector3& v);

    /**
     * @brief Constructs a Vector from a Point3.
     * @param v The point to convert into a vector.
     */
    Vector3(const Point3& v);

    /**
     * @brief Constructs a Vector from an initializer list of coordinates.
     * @param coords An initializer list containing the x, y, and z components.
     * @throws std::invalid_argument if the initializer list does not contain exactly three
     * elements.
     */
    Vector3(std::initializer_list<double> coords);

    /**
     * @brief Checks if two vectors are equal.
     * @param v The vector to compare with.
     * @return True if all components are equal, false otherwise.
     */
    bool operator==(const Vector3& v) const;

    /**
     * @brief Checks if two vectors are not equal.
     * @param v The vector to compare with.
     * @return True if any component differs, false otherwise.
     */
    bool operator!=(const Vector3& v) const;

    /**
     * @brief Assignment operator.
     * @param v The vector to assign from.
     * @return Reference to this vector.
     */
    Vector3& operator=(const Vector3& v);

    /**
     * @brief Negates the vector.
     * @return A new vector that is the negation of this vector.
     */
    Vector3 operator-() const;

    /**
     * @brief Adds two vectors.
     * @param v The vector to add.
     * @return The resulting vector.
     */
    Vector3 operator+(const Vector3& v) const;

    /**
     * @brief Adds another vector to this vector.
     * @param v The vector to add.
     * @return Reference to this vector after addition.
     */
    Vector3 operator+=(const Vector3& v);

    /**
     * @brief Adds a scalar to each component of the vector.
     * @param scalar The scalar value to add.
     * @return The resulting vector.
     */
    Vector3 operator+(const double scalar) const;

    /**
     * @brief Adds a scalar to each component of this vector.
     * @param scalar The scalar value to add.
     * @return Reference to this vector after addition.
     */
    Vector3 operator+=(const double scalar);

    /**
     * @brief Subtracts another vector from this vector.
     * @param v The vector to subtract.
     * @return The resulting vector.
     */
    Vector3 operator-(const Vector3& v) const;

    /**
     * @brief Subtracts another vector from this vector in place.
     * @param v The vector to subtract.
     * @return Reference to this vector after subtraction.
     */
    Vector3 operator-=(const Vector3& v);

    /**
     * @brief Subtracts a scalar from each component of the vector.
     * @param scalar The scalar value to subtract.
     * @return The resulting vector.
     */
    Vector3 operator-(const double scalar) const;

    /**
     * @brief Subtracts a scalar from each component of this vector.
     * @param scalar The scalar value to subtract.
     * @return Reference to this vector after subtraction.
     */
    Vector3 operator-=(const double scalar);

    /**
     * @brief Multiplies each component of the vector by a scalar.
     * @param scalar The scalar value to multiply by.
     * @return The resulting vector.
     */
    Vector3 operator*(double scalar) const;

    /**
     * @brief Multiplies each component of this vector by a scalar in place.
     * @param scalar The scalar value to multiply by.
     * @return Reference to this vector after multiplication.
     */
    Vector3 operator*=(double scalar);

    /**
     * @brief Divides each component of the vector by a scalar.
     * @param scalar The scalar value to divide by.
     * @return The resulting vector.
     * @throws std::invalid_argument if scalar is zero.
     */
    Vector3 operator/(double scalar) const;

    /**
     * @brief Divides each component of this vector by a scalar in place.
     * @param scalar The scalar value to divide by.
     * @return Reference to this vector after division.
     * @throws std::invalid_argument if scalar is zero.
     */
    Vector3 operator/=(double scalar);

    /**
     * @brief Computes the dot product with another vector.
     * @param v The vector to compute the dot product with.
     * @return The dot product (scalar).
     */
    double dot(const Vector3& v) const;

    /**
     * @brief Computes the dot product with another vector (operator overload).
     * @param v The vector to compute the dot product with.
     * @return The dot product (scalar).
     */
    double operator*(const Vector3& v) const;

    /**
     * @brief Computes the cross product with another vector.
     * @param v The vector to compute the cross product with.
     * @return The resulting vector.
     */
    Vector3 cross(const Vector3& v) const;

    /**
     * @brief Computes the cross product with another vector (operator overload).
     * @param v The vector to compute the cross product with.
     * @return The resulting vector.
     */
    Vector3 operator^(const Vector3& v) const;

    /**
     * @brief Computes the magnitude (length) of the vector.
     * @return The magnitude as a scalar.
     */
    double magnitude() const;

    /**
     * @brief Returns the normalized (unit) vector.
     * @return The normalized vector.
     * @throws std::invalid_argument if the vector is zero.
     */
    Vector3 normalize() const;

    double x; ///< The x component of the vector.
    double y; ///< The y component of the vector.
    double z; ///< The z component of the vector.
};

} // namespace Prism

#endif // PRISM_VECTOR_HPP_