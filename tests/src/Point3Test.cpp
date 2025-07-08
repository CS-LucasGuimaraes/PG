#include "Prism/matrix.hpp"
#include "Prism/point.hpp"
#include "Prism/utils.hpp"
#include "Prism/vector.hpp"
#include "TestHelpers.hpp"
#include <gtest/gtest.h>

using Prism::Matrix;
using Prism::Point3;
using Prism::Vector3;

TEST(Point3Test, ConstructorsAndAssignment) {
    Point3 p1(1.0, 2.0, 3.0);
    ASSERT_DOUBLE_EQ(p1.x, 1.0);
    ASSERT_DOUBLE_EQ(p1.y, 2.0);
    ASSERT_DOUBLE_EQ(p1.z, 3.0);

    Point3 p2 = p1;
    AssertPointAlmostEqual(p1, p2);

    Point3 p3(0, 0, 0);
    p3 = p1;
    AssertPointAlmostEqual(p3, p1);

    Point3 p4{4.0, 5.0, 6.0};
    AssertPointAlmostEqual(p4, Point3({4.0, 5.0, 6.0}));
}

TEST(Point3Test, Subtraction) {
    Point3 p1(5, 7, 9), p2(1, 2, 3);
    Vector3 v = p1 - p2;
    AssertVectorAlmostEqual(v, Vector3(4, 5, 6));

    Point3 p3(10, 10, 10);
    Vector3 v2 = p3 - p1;
    AssertVectorAlmostEqual(v2, Vector3(5, 3, 1));
}

TEST(Point3Test, Centroid) {
    Point3 p1(1, 2, 3);
    Point3 p2(4, 5, 6);
    Point3 p3(7, 8, 9);

    auto centroid = Prism::centroid({p1, p2, p3});
    AssertPointAlmostEqual(centroid, Point3(4, 5, 6));

    // Test with an empty list
    ASSERT_THROW(Prism::centroid({}), std::invalid_argument);
}

TEST(Point3Test, AdditionWithVector) {
    Point3 p1(1, 2, 3);
    Vector3 v(4, 5, 6);
    Point3 p2 = p1 + v;
    AssertPointAlmostEqual(p2, Point3(5, 7, 9));

    p1 += v;
    AssertPointAlmostEqual(p1, Point3(5, 7, 9));
}