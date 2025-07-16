#include "Prism.hpp"

#include "TestHelpers.hpp"

#include <gtest/gtest.h>

#include <vector>

using namespace Prism;

TEST(CameraTest, Instantiation) {
    Point3 position(1.0, 2.0, 3.0);
    Point3 target(4.0, 5.0, 6.0);
    Vector3 upvec(0.0, 1.0, 0.0);
    double distance = 10.0, height = 5.0, width = 8.0;

    Camera cam(position, target, upvec, distance, height, width, 10, 20);

    EXPECT_EQ(cam.pos.x, position.x);
    EXPECT_EQ(cam.pos.y, position.y);
    EXPECT_EQ(cam.pos.z, position.z);

    EXPECT_EQ(cam.aim.x, target.x);
    EXPECT_EQ(cam.aim.y, target.y);
    EXPECT_EQ(cam.aim.z, target.z);

    EXPECT_EQ(cam.up.x, upvec.x);
    EXPECT_EQ(cam.up.y, upvec.y);
    EXPECT_EQ(cam.up.z, upvec.z);

    EXPECT_DOUBLE_EQ(cam.screen_distance, distance);
    EXPECT_DOUBLE_EQ(cam.screen_height, height);
    EXPECT_DOUBLE_EQ(cam.screen_width, width);
}

TEST(CameraTest, IteratorGeneratesCorrectNumberOfRays) {
    Point3 position(0, 0, 0);
    Point3 target(0, 0, -1);
    Vector3 upvec(0, 1, 0);
    int p_height = 10, p_width = 20;

    Camera cam(position, target, upvec, 1.0, 2.0, 2.0, p_height, p_width);

    int count = 0;
    for (const auto& ray : cam) {
        (void)ray; // Evita aviso de variável não utilizada
        count++;
    }

    ASSERT_EQ(count, p_height * p_width);
}

TEST(CameraTest, IteratorGeneratesGeometricallyCorrectRays) {
    const double aspect_ratio = 16.0 / 9.0;
    const int image_width = 160, image_height = 90;
    const double viewport_height = 2.0;
    const double viewport_width = viewport_height * aspect_ratio;
    const double distance_to_screen = 1.0;

    Point3 pos(0, 0, 0);
    Point3 target(0, 0, -1);
    Vector3 up(0, 1, 0);

    Camera cam(pos, target, up, distance_to_screen, viewport_height, viewport_width, image_height,
               image_width);

    std::vector<Ray> generated_rays;
    for (const auto& r : cam) {
        generated_rays.push_back(r);
    }

    ASSERT_EQ(generated_rays.size(), image_width * image_height);

    Point3 expected_top_left(-viewport_width / 2.0 + (viewport_width / image_width) * 0.5,
                             viewport_height / 2.0 - (viewport_height / image_height) * 0.5,
                             -distance_to_screen);

    Point3 expected_bottom_right(viewport_width / 2.0 - (viewport_width / image_width) * 0.5,
                                 -viewport_height / 2.0 + (viewport_height / image_height) * 0.5,
                                 -distance_to_screen);

    auto get_intersection_point = [&](const Ray& ray) {
        double t = (-distance_to_screen - ray.origin().z) / ray.direction().z;
        return ray.origin() + (ray.direction() * t);
    };

    Ray& top_left_ray = generated_rays.front();
    Ray& bottom_right_ray = generated_rays.back();

    Point3 actual_top_left = get_intersection_point(top_left_ray);
    Point3 actual_bottom_right = get_intersection_point(bottom_right_ray);

    AssertPointAlmostEqual(actual_top_left, expected_top_left);
    AssertPointAlmostEqual(actual_bottom_right, expected_bottom_right);
}