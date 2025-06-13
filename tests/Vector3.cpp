#include "Prism/vector.hpp"
#include "TestHelpers.hpp"
#include <gtest/gtest.h>

using Prism::Vector3;

TEST(Vector3Test, ConstructorsAndAssignment) {
    Vector3 v1(1.0, 2.0, 3.0);
    ASSERT_DOUBLE_EQ(v1.x, 1.0);
    ASSERT_DOUBLE_EQ(v1.y, 2.0);
    ASSERT_DOUBLE_EQ(v1.z, 3.0);

    Vector3 v2 = v1;
    ASSERT_EQ(v1, v2);

    Vector3 v3(0, 0, 0);
    v3 = v1;
    ASSERT_EQ(v3, v1);
    Vector3 v4({4.0, 5.0, 6.0});
    ASSERT_EQ(v4, Vector3({4.0, 5.0, 6.0}));
}

TEST(Vector3Test, EqualityOperators) {
    Vector3 v1(1, 2, 3), v2(1, 2, 3), v3(3, 2, 1);
    ASSERT_EQ(v1, v2);
    ASSERT_NE(v1, v3);
    ASSERT_TRUE(v1 == v2);
    ASSERT_FALSE(v1 == v3);
}

TEST(Vector3Test, Addition) {
    Vector3 v1(1, 2, 3), v2(4, 5, 6);
    Vector3 v3 = v1 + v2;
    ASSERT_EQ(v3, Vector3(5, 7, 9));

    v1 += v2;
    ASSERT_EQ(v1, Vector3(5, 7, 9));
}

TEST(Vector3Test, Subtraction) {
    Vector3 v1(5, 7, 9), v2(1, 2, 3);
    Vector3 v3 = v1 - v2;
    ASSERT_EQ(v3, Vector3(4, 5, 6));

    v1 -= v2;
    ASSERT_EQ(v1, Vector3(4, 5, 6));
}

TEST(Vector3Test, ScalarMultiplicationAndDivision) {
    Vector3 v1(1, 2, 3);
    Vector3 v2 = v1 * 2.0;
    ASSERT_EQ(v2, Vector3(2, 4, 6));

    v2 *= 0.5;
    AssertVectorAlmostEqual(v2, Vector3(1, 2, 3));

    Vector3 v3 = v1 / 2.0;
    AssertVectorAlmostEqual(v3, Vector3(0.5, 1, 1.5));

    ASSERT_THROW(v1 / 0.0, std::invalid_argument);
    ASSERT_THROW(v1 /= 0.0, std::invalid_argument);
}

TEST(Vector3Test, DotAndCrossProduct) {
    Vector3 v1(1, 2, 3), v2(4, -5, 6);

    double expected_dot = 1 * 4 + 2 * (-5) + 3 * 6;
    ASSERT_NEAR(v1.dot(v2), expected_dot, 1e-9);
    ASSERT_NEAR(v1 * v2, expected_dot, 1e-9);

    Vector3 cross = v1.cross(v2);
    Vector3 expected_cross(27, 6, -13);
    AssertVectorAlmostEqual(cross, expected_cross);
}

TEST(Vector3Test, MagnitudeAndNormalize) {
    Vector3 v1(3, 4, 0);
    ASSERT_NEAR(v1.magnitude(), 5.0, 1e-9);

    Vector3 norm = v1.normalize();
    ASSERT_NEAR(norm.magnitude(), 1.0, 1e-9);
    AssertVectorAlmostEqual(norm, Vector3(0.6, 0.8, 0.0));

    Vector3 zero(0, 0, 0);
    ASSERT_THROW(zero.normalize(), std::invalid_argument);
}