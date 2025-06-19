#include "Prism/utils.hpp"
#include "Prism/point.hpp"
#include "Prism/vector.hpp"
#include "TestHelpers.hpp"
#include <gtest/gtest.h>
#include <vector>

using Prism::centroid;
using Prism ::Matrix;
using Prism::orthonormalBasisContaining;
using Prism::Point3;
using Prism ::Vector3;

TEST(UtilsTest, CentroidOfMultiplePoints) {
    Point3 p1(0.0, 0.0, 0.0);
    Point3 p2(9.0, 3.0, 0.0);
    Point3 p3(0.0, 6.0, 12.0);

    Point3 expected_centroid(3.0, 3.0, 4.0);

    Point3 calculated_centroid = centroid({p1, p2, p3});

    AssertPointAlmostEqual(calculated_centroid, expected_centroid);
}

TEST(UtilsTest, CentroidOfSinglePoint) {
    Point3 p1(10.5, -20.0, 30.1);

    Point3 expected_centroid(10.5, -20.0, 30.1);

    Point3 calculated_centroid = centroid({p1});

    AssertPointAlmostEqual(calculated_centroid, expected_centroid);
}

TEST(UtilsTest, CentroidOfSymmetricPoints) {
    Point3 p1(10.0, 10.0, 10.0);
    Point3 p2(-10.0, -10.0, -10.0);

    Point3 expected_centroid(0.0, 0.0, 0.0);

    Point3 calculated_centroid = centroid({p1, p2});

    AssertPointAlmostEqual(calculated_centroid, expected_centroid);
}

TEST(UtilsTest, CentroidThrowsOnEmptyList) {
    // Verify that calling centroid with an empty list of points throws an invalid_argument
    // exception.
    ASSERT_THROW(centroid({}), std::invalid_argument);
}

// Helper function to extract a column vector from a 3x3 matrix
Vector3 GetColumn(const Matrix& m, int col) {
    return Vector3{m[0][col], m[1][col], m[2][col]};
}

TEST(OrthonormalBasisTest, ProducesOrthonormalBasisContainingVector) {
    Vector3 input{1, 2, 3};
    Matrix basis = orthonormalBasisContaining(input);

    Vector3 v1 = GetColumn(basis, 0);
    Vector3 v2 = GetColumn(basis, 1);
    Vector3 v3 = GetColumn(basis, 2);

    // Verify vectors are unit length
    ASSERT_NEAR(v1.magnitude(), 1.0, 1e-9);
    ASSERT_NEAR(v2.magnitude(), 1.0, 1e-9);
    ASSERT_NEAR(v3.magnitude(), 1.0, 1e-9);

    // Verify vectors are orthogonal
    ASSERT_NEAR(v1.dot(v2), 0.0, 1e-9);
    ASSERT_NEAR(v1.dot(v3), 0.0, 1e-9);
    ASSERT_NEAR(v2.dot(v3), 0.0, 1e-9);

    // Verify that v1 is in the same direction as input.normalized()
    Vector3 normalized_input = input.normalize();
    ASSERT_NEAR(v1.dot(normalized_input), 1.0, 1e-9); // Should be same direction
}