#include "Prism/camera.hpp"
#include "Prism/matrix.hpp"
#include "Prism/point.hpp"
#include "Prism/ray.hpp"
#include "Prism/utils.hpp"
#include "Prism/vector.hpp"
#include "TestHelpers.hpp"
#include <gtest/gtest.h>
#include <vector>

using Prism::Camera;
using Prism::Matrix;
using Prism::Point3;
using Prism::Ray;
using Prism::Vector3;

TEST(CameraTest, Instantiation) {
    // Arrange: create Point3 and Vector3 for constructor parameters
    Point3 position(1.0L, 2.0L, 3.0L);
    Point3 target(4.0L, 5.0L, 6.0L);
    Vector3 upvec(0.0L, 1.0L, 0.0L);

    double distance = 10.0L;
    double height = 5.0L;
    double width = 8.0L;

    // Act: instantiate the Camera
    Camera cam(position, target, upvec, distance, height, width, 10, 20);

    // Assert: verify that pointers are not null and values match
    ASSERT_NE(cam.pos, nullptr);
    ASSERT_NE(cam.aim, nullptr);
    ASSERT_NE(cam.up, nullptr);
    ASSERT_NE(cam.coordinate_basis, nullptr); // Assuming constructor initializes this

    EXPECT_EQ(cam.pos->x, position.x);
    EXPECT_EQ(cam.pos->y, position.y);
    EXPECT_EQ(cam.pos->z, position.z);

    EXPECT_EQ(cam.aim->x, target.x);
    EXPECT_EQ(cam.aim->y, target.y);
    EXPECT_EQ(cam.aim->z, target.z);

    EXPECT_EQ(cam.up->x, upvec.x);
    EXPECT_EQ(cam.up->y, upvec.y);
    EXPECT_EQ(cam.up->z, upvec.z);

    EXPECT_DOUBLE_EQ(cam.screen_distance, distance);
    EXPECT_DOUBLE_EQ(cam.screen_height, height);
    EXPECT_DOUBLE_EQ(cam.screen_width, width);
}

TEST(CameraTest, CoordinateBasisOrthonormal) {
    // Arrange
    Point3 position(1.0L, 2.0L, 3.0L);
    Point3 target(4.0L, 5.0L, 6.0L);
    Vector3 upvec(0.0L, 1.0L, 0.0L);

    double distance = 10.0L;
    double height = 5.0L;
    double width = 8.0L;

    // Act
    Camera cam(position, target, upvec, distance, height, width, 10, 20);
    ASSERT_NE(cam.coordinate_basis, nullptr);

    // Extract basis matrix and columns
    const Matrix<double>& basis = *cam.coordinate_basis;
    auto getCol = [&](int c) { return Vector3{basis[0][c], basis[1][c], basis[2][c]}; };

    // Expected first basis vector: normalized (position - target)
    Vector3 dir = (position - target).normalize();
    Vector3 b1 = getCol(0);
    Vector3 b2 = getCol(1);
    Vector3 b3 = getCol(2);

    // Assert alignment of first basis
    AssertVectorAlmostEqual(b1, dir);

    // Assert unit length
    ASSERT_NEAR(b1.magnitude(), 1.0L, 1e-9);
    ASSERT_NEAR(b2.magnitude(), 1.0L, 1e-9);
    ASSERT_NEAR(b3.magnitude(), 1.0L, 1e-9);

    // Assert orthogonality
    ASSERT_NEAR(b1.dot(b2), 0.0L, 1e-9);
    ASSERT_NEAR(b1.dot(b3), 0.0L, 1e-9);
    ASSERT_NEAR(b2.dot(b3), 0.0L, 1e-9);
}

TEST(CameraTest,
     BasisHasOpposite) { // verifies if basis contains vector opposite to target - position
    Point3 position(1.0L, 2.0L, 3.0L);
    Point3 target(4.0L, 5.0L, 6.0L);
    Vector3 upvec(0.0L, 1.0L, 0.0L);

    double distance = 10.0L;
    double height = 5.0L;
    double width = 8.0L;

    // Act
    Camera cam(position, target, upvec, distance, height, width, 10, 20);
    ASSERT_NE(cam.coordinate_basis, nullptr);

    // Extract basis matrix and columns
    const Matrix<double>& basis = *cam.coordinate_basis;
    auto getCol = [&](int c) { return Vector3{basis[0][c], basis[1][c], basis[2][c]}; };

    // Expected first basis vector: normalized (position - target)
    Vector3 dir = (position - target).normalize();
    Vector3 b1 = getCol(0);
    Vector3 b2 = getCol(1);
    Vector3 b3 = getCol(2);

    AssertVectorAlmostEqual(b1, ((target - position) * -1).normalize());
}

TEST(CameraTest, IteratorGeneratesCorrectNumberOfRays) {
    Point3 position(0, 0, 0);
    Point3 target(0, 0, -1);
    Vector3 upvec(0, 1, 0);
    int p_height = 10;
    int p_width = 20;

    Camera cam(position, target, upvec, 1.0L, 2.0L, 2.0L, p_height, p_width);

    int count = 0;
    for (const auto& ray : cam) {
        (void)ray;
        count++;
    }

    ASSERT_EQ(count, p_height * p_width);
}

TEST(CameraTest, IteratorGeneratesGeometricallyCorrectRays) {
    const double aspect_ratio = 16.0L / 9.0L;
    const int image_width = 160;
    const int image_height = 90;

    const double viewport_height = 2.0L;
    const double viewport_width = viewport_height * aspect_ratio;
    const double distance_to_screen = 1.0L;

    Prism::Point3 pos(0, 0, 0);
    Prism::Point3 target(0, 0, -1);
    Prism::Vector3 up(0, 1, 0);

    Prism::Camera cam(pos, target, up, distance_to_screen, viewport_height, viewport_width,
                      image_height, image_width);

    std::vector<Prism::Ray> generated_rays;
    for (const auto& r : cam) {
        generated_rays.push_back(r);
    }

    ASSERT_EQ(generated_rays.size(), image_width * image_height);

    Prism::Point3 expected_top_left(
        -viewport_width / 2.0L + (viewport_width / image_width) * 0.5L,
        viewport_height / 2.0L - (viewport_height / image_height) * 0.5L, -distance_to_screen);

    Prism::Point3 expected_bottom_right(
        viewport_width / 2.0L - (viewport_width / image_width) * 0.5L,
        -viewport_height / 2.0L + (viewport_height / image_height) * 0.5L, -distance_to_screen);

    auto get_intersection_point = [&](const Prism::Ray& ray) {
        double t = (-distance_to_screen - ray.origin().z) / ray.direction().z;
        return ray.origin() + (ray.direction() * t);
    };

    Prism::Ray& top_left_ray = generated_rays.front();
    Prism::Ray& bottom_right_ray = generated_rays.back();

    Prism::Point3 actual_top_left = get_intersection_point(top_left_ray);
    Prism::Point3 actual_bottom_right = get_intersection_point(bottom_right_ray);

    AssertPointAlmostEqual(actual_top_left, expected_top_left);
    AssertPointAlmostEqual(actual_bottom_right, expected_bottom_right);
}

TEST(CameraTest, IteratorGeneratesCorrectRayForAllPixels) {
    const int image_width = 80;
    const int image_height = 45;
    const double aspect_ratio = double(image_width) / double(image_height);

    const double viewport_height = 2.0L;
    const double viewport_width = viewport_height * aspect_ratio;
    const double distance_to_screen = 1.0L;

    Prism::Point3 pos(0, 0, 1);
    Prism::Point3 target(0, 0, 0);
    Prism::Vector3 up(0, 1, 0);

    Prism::Camera cam(pos, target, up, distance_to_screen, viewport_height, viewport_width,
                      image_height, image_width);

    std::vector<Prism::Ray> generated_rays;
    generated_rays.reserve(image_width * image_height); // Otimização para evitar realocações
    for (const auto& r : cam) {
        generated_rays.push_back(r);
    }

    ASSERT_EQ(generated_rays.size(), image_width * image_height);

    Prism::Matrix<double> basis = Prism::orthonormalBasisContaining(pos - target);
    Prism::Vector3 u_vec = {basis[0][1], basis[1][1], basis[2][1]};
    Prism::Vector3 v_vec = {basis[0][2], basis[1][2], basis[2][2]};
    Prism::Vector3 w_vec = {basis[0][0], basis[1][0], basis[2][0]};

    Prism::Point3 screen_center = pos - (w_vec * distance_to_screen);
    Prism::Point3 top_left_corner =
        screen_center - (u_vec * (viewport_width / 2.0)) + (v_vec * (viewport_height / 2.0));

    Prism::Vector3 expected_delta_u = u_vec * (viewport_width / image_width);
    Prism::Vector3 expected_delta_v = v_vec * (viewport_height / image_height);
    Prism::Point3 expected_pixel_00 =
        top_left_corner + (expected_delta_u * 0.5) - (expected_delta_v * 0.5);

    auto get_intersection_point = [&](const Prism::Ray& ray) {
        double t = (target - ray.origin()).dot(w_vec) / (ray.direction().dot(w_vec));
        return ray.origin() + (ray.direction() * t);
    };

    int ray_index = 0;
    for (int y = 0; y < image_height; ++y) {
        for (int x = 0; x < image_width; ++x) {
            const Prism::Ray& current_ray = generated_rays[ray_index];

            Prism::Point3 expected_target =
                expected_pixel_00 + (expected_delta_u * x) - (expected_delta_v * y);

            Prism::Point3 actual_target = get_intersection_point(current_ray);

            AssertPointAlmostEqual(actual_target, expected_target);

            ray_index++;
        }
    }
}