/**
 * @file utils.hpp
 * @brief Prism Render Engine
 *
 * @copyright Copyright (c) 2025 src-lua
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef PRISM_UTILS_HPP_
#define PRISM_UTILS_HPP_

#include "prism_export.h"

#include "Prism/core/matrix.hpp"
#include "Prism/core/point.hpp"
#include "Prism/core/vector.hpp"

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
Matrix PRISM_EXPORT orthonormalBasisContaining(const Vector3& vec);

/**
 * @brief Computes the square of a value.
 * @param value The value to square.
 * @return The square of the value.
 */
template <typename T> T sqr(const T& value) {
    return value * value;
}

/**
 * @brief Computes the refracted vector based on Snell's law.
 * This function calculates the refracted vector given an incident vector, a normal vector,
 * and the ratio of indices of refraction.
 * @param uv The incident vector (unit vector).
 * @param n The normal vector at the point of incidence (unit vector).
 * @param etai_over_etat The ratio of indices of refraction (etai/etat).
 * @return The refracted vector as a Vector3.
 */
Vector3 PRISM_EXPORT refract(const Vector3& uv, const Vector3& n, double etai_over_etat);

/**
 * @brief Calcs the reflectance using Schlick's approximation.
 * This function computes the reflectance based on the angle of incidence and the index of
 * refraction using Schlick's approximation.
 * @param cosine The cosine of the angle between the incident vector and the normal.
 * @param ref_idx The index of refraction of the material.
 * @return The reflectance value as a double, which is the probability of reflection at the
 * interface.
 * This value is used in ray tracing to determine how much light is reflected versus refracted.
 */
double PRISM_EXPORT schlick(double cosine, double ref_idx);

} // namespace Prism

#endif // PRISM_UTILS_HPP_