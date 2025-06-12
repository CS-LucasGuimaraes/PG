#ifndef PRISM_UTILS_HPP_
#define PRISM_UTILS_HPP_

#include "Prism/matrix.hpp"
#include "Prism/point.hpp"
#include "Prism/vector.hpp"
#include "prism_export.h"
#include <initializer_list>

namespace Prism {

/**
 * @brief Computes the centroid of a set of points in 3D space.
 * This function calculates the centroid (geometric center) of a collection of points
 * by averaging their coordinates.
 * @param points An initializer list of Point3 objects representing the points in 3D space.
 * @return A Point3 object representing the centroid of the given points.
 */
Point3 PRISM_EXPORT centroid(const std::initializer_list<Point3>& points);

/**
 * @brief Computes an orthonormal basis containing a given vector.
 * This function constructs a 3x3 matrix where the first column is the normalized
 * version of the input vector, the second column is orthogonal to the first,
 * and the third column is the cross product of the first two.
 * @param vec The input vector to construct the basis around.
 * @return A 3x3 matrix representing the orthonormal basis.
 * The first column is the normalized input vector, the second is orthogonal to it,
 * and the third is the cross product of the first two.
 * @throws std::invalid_argument if the input vector is zero.
 */
Matrix<ld> PRISM_EXPORT orthonormalBasisContaining(const Vector3& vec);

/**
 * @brief Computes the square of a value.
 * @param value The value to square.
 * @return The square of the value.
 */
template <typename T>
T sqr(const T& value) {
    return value * value;
}

// namespace Prism

#endif // PRISM_UTILS_HPP_