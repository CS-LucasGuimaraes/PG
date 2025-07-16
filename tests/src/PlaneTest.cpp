/**
 * @file PlaneTest.cpp
 * @brief Prism Render Engine
 *
 * @copyright Copyright (c) 2025 src-lua
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "Prism.hpp"

#include "TestHelpers.hpp"

#include <gtest/gtest.h>

#include <memory>

using namespace Prism;

TEST(PlaneTest, RayHitsPlane) {
    Point3 plane_point(0.0, 0.0, 0.0);
    Vector3 normal(0.0, 1.0, 0.0);
    auto mat = std::make_shared<Material>(Color(1.0f, 0.0f, 0.0f));
    Plane plane(plane_point, normal, mat);

    Point3 ray_origin(0.0, -1.0, 0.0);
    Vector3 ray_dir(0.0, 1.0, 0.0);
    Ray ray(ray_origin, ray_dir);

    HitRecord rec;
    bool hit = plane.hit(ray, 0.001, 1000.0, rec);

    EXPECT_TRUE(hit);
    EXPECT_NEAR(rec.t, 1.0, 1e-9);
    AssertPointAlmostEqual(rec.p, Point3(0.0, 0.0, 0.0));
    AssertVectorAlmostEqual(rec.normal, Vector3(0.0, -1.0, 0.0)); // Normal aponta contra o raio
    EXPECT_EQ(rec.material, mat);
}

TEST(PlaneTest, RayIsParallelAndOutsidePlane) {
    Point3 plane_point(0.0, 0.0, 0.0);
    Vector3 normal(0.0, 1.0, 0.0);
    auto mat = std::make_shared<Material>(Color(1.0f, 0.0f, 0.0f));
    Plane plane(plane_point, normal, mat);

    Point3 ray_origin(0.0, 1.0, 0.0);
    Vector3 ray_dir(1.0, 0.0, 0.0);
    Ray ray(ray_origin, ray_dir);

    HitRecord rec;
    EXPECT_FALSE(plane.hit(ray, 0.001, 1000.0, rec));
}

TEST(PlaneTest, RayIsParallelAndOnPlane) {
    Point3 plane_point(0.0, 0.0, 0.0);
    Vector3 normal(0.0, 1.0, 0.0);
    auto mat = std::make_shared<Material>(Color(1.0f, 0.0f, 0.0f));
    Plane plane(plane_point, normal, mat);

    Point3 ray_origin(0.0, 0.0, 0.0); // Origem no próprio plano
    Vector3 ray_dir(1.0, 0.0, 0.0);
    Ray ray(ray_origin, ray_dir);

    HitRecord rec;
    EXPECT_FALSE(plane.hit(ray, 0.001, 1000.0, rec));
}

TEST(PlaneTest, RayPointsAwayFromPlane) {
    Point3 plane_point(0.0, 0.0, 0.0);
    Vector3 normal(0.0, 1.0, 0.0);
    auto mat = std::make_shared<Material>(Color(1.0f, 0.0f, 0.0f));
    Plane plane(plane_point, normal, mat);

    Point3 ray_origin(0.0, 1.0, 0.0);
    Vector3 ray_dir(0.0, 1.0, 0.0); // Aponta para longe do plano
    Ray ray(ray_origin, ray_dir);

    HitRecord rec;
    EXPECT_FALSE(plane.hit(ray, 0.001, 1000.0, rec));
}

TEST(PlaneTest, RayHitsPlaneBehindOrigin) {
    Point3 plane_point(0.0, 0.0, 0.0);
    Vector3 normal(0.0, 1.0, 0.0);
    auto mat = std::make_shared<Material>(Color(1.0f, 0.0f, 0.0f));
    Plane plane(plane_point, normal, mat);

    Point3 ray_origin(0.0, 1.0, 0.0);
    Vector3 ray_dir(0.0, 1.0, 0.0); // Aponta para longe
    Ray ray(ray_origin, ray_dir);

    HitRecord rec;
    // A interseção matemática ocorreria em t = -1, que é menor que t_min.
    EXPECT_FALSE(plane.hit(ray, 0.001, 1000.0, rec));
}