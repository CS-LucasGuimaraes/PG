#include "Prism/ray.hpp"
#include "Prism/objects.hpp"
#include "Prism/point.hpp"
#include "Prism/vector.hpp"
#include "Prism/plane.hpp"
#include "Prism/material.hpp"
#include "TestHelpers.hpp"
#include <gtest/gtest.h>
#include "Prism/color.hpp"

using namespace Prism;


TEST(PlaneTest, RayHitsPlane) {
    Point3 plane_point(0.0L, 0.0L, 0.0L);
    Vector3 normal(0.0L, 1.0L, 0.0L);
    Material mat(Color(1.0L, 0.0L, 0.0L));
    Plane plane(plane_point, normal, &mat);

    Point3 ray_origin(0.0L, -1.0L, 0.0L);
    Vector3 ray_dir(0.0L, 1.0L, 0.0L);
    Ray ray(ray_origin, ray_dir);

    HitRecord rec;
    bool hit = plane.hit(ray, 0.001L, 1000.0L, rec);

    EXPECT_TRUE(hit);
    EXPECT_NEAR(rec.t, 1.0L, 1e-9);
    EXPECT_NEAR(rec.p.x, 0.0L, 1e-9);
    EXPECT_NEAR(rec.p.y, 0.0L, 1e-9);
    EXPECT_NEAR(rec.p.z, 0.0L, 1e-9);
    EXPECT_NEAR(rec.normal.x, 0.0L, 1e-9);
    EXPECT_NEAR(rec.normal.y, 1.0L, 1e-9);
    EXPECT_NEAR(rec.normal.z, 0.0L, 1e-9);
    EXPECT_EQ(rec.material, &mat);
}

TEST(PlaneTest, RayParallelToPlane) {
    Point3 plane_point(0.0L, 0.0L, 0.0L);
    Vector3 normal(0.0L, 1.0L, 0.0L);
    Material mat(Color(1.0L, 0.0L, 0.0L));  
    Plane plane(plane_point, normal, &mat);

    Point3 ray_origin(0.0L, 1.0L, 0.0L);
    Vector3 ray_dir(1.0L, 0.0L, 0.0L); // Parallel to plane
    Ray ray(ray_origin, ray_dir);

    HitRecord rec;
    bool hit = plane.hit(ray, 0.001L, 1000.0L, rec);

    EXPECT_FALSE(hit);
}

TEST(PlaneTest, RayMissesPlane) {
    Point3 plane_point(0.0L, 0.0L, 0.0L);
    Vector3 normal(0.0L, 1.0L, 0.0L);
    Material mat(Color(1.0L, 0.0L, 0.0L));
    Plane plane(plane_point, normal, &mat);

    Point3 ray_origin(0.0L, 1.0L, 0.0L);
    Vector3 ray_dir(0.0L, 1.0L, 0.0L); // Points away from plane
    Ray ray(ray_origin, ray_dir);

    HitRecord rec;
    bool hit = plane.hit(ray, 0.001L, 1000.0L, rec);

    EXPECT_FALSE(hit);
}

TEST(PlaneTest, RayHitsPlaneBehindOrigin) {
    Point3 plane_point(0.0L, 0.0L, 0.0L);
    Vector3 normal(0.0L, 1.0L, 0.0L);
    Material mat(Color(1.0L, 0.0L, 0.0L));
    Plane plane(plane_point, normal, &mat);

    Point3 ray_origin(0.0L, 1.0L, 0.0L);
    Vector3 ray_dir(0.0L, 1.0L, 0.0L); // Points away from plane
    Ray ray(ray_origin, ray_dir);

    HitRecord rec;
    // t_min = 0.001, intersection would be at t = -1, so should not hit
    bool hit = plane.hit(ray, 0.001L, 1000.0L, rec);

    EXPECT_FALSE(hit);
}

TEST(PlaneTest, RayHitsPlaneAtTMin) {
    Point3 plane_point(0.0L, 0.0L, 0.0L);
    Vector3 normal(0.0L, 1.0L, 0.0L);
    Material mat(Color(1.0L, 0.0L, 0.0L));
    Plane plane(plane_point, normal, &mat);

    Point3 ray_origin(0.0L, -0.001L, 0.0L);
    Vector3 ray_dir(0.0L, 1.0L, 0.0L);
    Ray ray(ray_origin, ray_dir);

    HitRecord rec;
    bool hit = plane.hit(ray, 0.001L, 1000.0L, rec);

    EXPECT_TRUE(hit);
    EXPECT_NEAR(rec.t, 0.001L, 1e-9);
    EXPECT_NEAR(rec.p.y, 0.0L, 1e-9);
}
