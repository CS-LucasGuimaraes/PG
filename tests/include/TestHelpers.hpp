/**
 * @file TestHelpers.hpp
 * @brief Prism Render Engine
 *
 * @copyright Copyright (c) 2025 src-lua
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef TESTS_TESTHELPERS_HPP
#define TESTS_TESTHELPERS_HPP

#include "Prism.hpp"

#include <gtest/gtest.h>

namespace Prism {

/**
 * @brief Asserts that two Point3 objects are almost equal within a given epsilon.
 * @param p1 The first point to compare.
 * @param p2 The second point to compare.
 * @param eps The epsilon value for comparison (default is 1e-9).
 * @note This function compares each corresponding component using ASSERT_NEAR.
 */
inline void AssertPointAlmostEqual(const Point3& p1, const Point3& p2, double eps = 1e-9) {
    ASSERT_NEAR(p1.x, p2.x, eps);
    ASSERT_NEAR(p1.y, p2.y, eps);
    ASSERT_NEAR(p1.z, p2.z, eps);
}

/**
 * @brief Asserts that two Vector3 objects are almost equal within a given epsilon.
 * @param v1 The first vector to compare.
 * @param v2 The second vector to compare.
 * @param eps The epsilon value for comparison (default is 1e-9).
 * @note This function compares each corresponding component using ASSERT_NEAR.
 */
inline void AssertVectorAlmostEqual(const Vector3& v1, const Vector3& v2, double eps = 1e-9) {
    ASSERT_NEAR(v1.x, v2.x, eps);
    ASSERT_NEAR(v1.y, v2.y, eps);
    ASSERT_NEAR(v1.z, v2.z, eps);
}

/**
 * @brief Asserts that two Matrix objects are almost equal within a given epsilon.
 * @param m1 The first matrix to compare.
 * @param m2 The second matrix to compare.
 * @param eps The epsilon value for comparison (default is 1e-9).
 * @tparam T The type of elements in the matrix (should support near comparison).
 * @throws std::out_of_range if the matrices have different dimensions.
 * @throws std::invalid_argument if the matrices are empty.
 * @throws std::invalid_argument if the matrices have different dimensions.
 * @note This function checks that both matrices have the same number of rows and columns,
 *       and then compares each corresponding element using ASSERT_NEAR.
 */
inline void AssertMatrixAlmostEqual(const Matrix& m1, const Matrix& m2, double eps = 1e-9) {
    ASSERT_EQ(m1.getRows(), m2.getRows());
    ASSERT_EQ(m1.getCols(), m2.getCols());
    for (size_t i = 0; i < m1.getRows(); ++i) {
        for (size_t j = 0; j < m1.getCols(); ++j) {
            ASSERT_NEAR(m1[i][j], m2[i][j], eps);
        }
    }
}

} // namespace Prism

#endif // TESTS_TESTHELPERS_HPP