#include "Prism.hpp"
#include "TestHelpers.hpp"
#include <cmath>
#include <gtest/gtest.h>
#include <memory>

using namespace Prism;

TEST(SphereTest, HitFromOutside) {
    Point3 center(0.0L, 0.0L, 0.0L);
    auto material = std::make_shared<Material>(Color(1.0f, 0.0f, 0.0f));
    Sphere sphere(center, 1.0L, material);

    Ray ray(Point3(0.0L, 0.0L, -5.0L), Vector3(0.0L, 0.0L, 1.0L));
    HitRecord rec;

    EXPECT_TRUE(sphere.hit(ray, 0.001L, INFINITY, rec));
    EXPECT_DOUBLE_EQ(rec.t, 4.0L);

    AssertPointAlmostEqual(rec.p, Point3(0.0L, 0.0L, -1.0L));

    AssertVectorAlmostEqual(rec.normal, Vector3(0.0L, 0.0L, -1.0L));
    EXPECT_TRUE(rec.front_face);
}

TEST(SphereTest, Miss) {
    Point3 center(0.0L, 0.0L, 0.0L);
    auto material = std::make_shared<Material>(Color(1.0f, 0.0f, 0.0f));
    Sphere sphere(center, 1.0L, material);

    Ray ray(Point3(0.0L, 2.0L, -5.0L), Vector3(0.0L, 0.0L, 1.0L));
    HitRecord rec;

    EXPECT_FALSE(sphere.hit(ray, 0.001L, INFINITY, rec));
}

TEST(SphereTest, Tangent) {
    Point3 center(0.0L, 0.0L, 0.0L);
    auto material = std::make_shared<Material>(Color(1.0f, 0.0f, 0.0f));
    Sphere sphere(center, 1.0L, material);

    Ray ray(Point3(0.0L, 1.0L, -5.0L), Vector3(0.0L, 0.0L, 1.0L));
    HitRecord rec;

    EXPECT_TRUE(sphere.hit(ray, 0.001L, INFINITY, rec));
    EXPECT_NEAR(rec.t, 5.0L, 1e-7);
    AssertPointAlmostEqual(rec.p, Point3(0.0L, 1.0L, 0.0L));
}

TEST(SphereTest, RayStartsInside) {
    Point3 center(0.0L, 0.0L, 0.0L);
    auto material = std::make_shared<Material>(Color(1.0f, 0.0f, 0.0f));
    Sphere sphere(center, 1.0L, material);

    Ray ray(center, Vector3(0.0L, 0.0L, 1.0L));
    HitRecord rec;

    EXPECT_TRUE(sphere.hit(ray, 0.001L, INFINITY, rec));
    EXPECT_DOUBLE_EQ(rec.t, 1.0L);

    AssertPointAlmostEqual(rec.p, Point3(0.0L, 0.0L, 1.0L));

    AssertVectorAlmostEqual(rec.normal, Vector3(0.0L, 0.0L, -1.0L));
    EXPECT_FALSE(rec.front_face);
}

TEST(SphereTest, HitBehindRay) {
    Point3 center(0.0L, 0.0L, 0.0L);
    auto material = std::make_shared<Material>(Color(1.0f, 0.0f, 0.0f));
    Sphere sphere(center, 1.0L, material);

    Ray ray(Point3(0.0L, 0.0L, 5.0L), Vector3(0.0L, 0.0L, 1.0L));
    HitRecord rec;

    EXPECT_FALSE(sphere.hit(ray, 0.001L, INFINITY, rec));
}