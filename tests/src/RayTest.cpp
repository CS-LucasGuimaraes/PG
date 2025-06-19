#include "Prism/ray.hpp"
#include "Prism/objects.hpp"
#include "Prism/point.hpp"
#include "Prism/vector.hpp"
#include "TestHelpers.hpp"
#include <gtest/gtest.h>
#include <memory>
#include <vector>

using namespace Prism;
using std::vector;

class DummyObject : public Object {
  public:
    bool hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const override {
        rec.t = 2.0L;
        rec.p = Point3(1.0L, 1.0L, 1.0L);
        rec.normal = Vector3(0.0L, 1.0L, 0.0L);
        rec.material = nullptr;
        rec.set_face_normal(ray, rec.normal);
        return true;
    }
};

TEST(RayTest, ConstructorWithDirection) {
    Point3 origin(0.0L, 0.0L, 0.0L);
    Vector3 dir(1.0L, 0.0L, 0.0L);
    Ray ray(origin, dir);

    EXPECT_DOUBLE_EQ(ray.origin().x, 0.0L);
    EXPECT_DOUBLE_EQ(ray.direction().x, 1.0L);
}

TEST(RayTest, ConstructorWithTarget) {
    Point3 origin(0.0L, 0.0L, 0.0L);
    Point3 target(0.0L, 0.0L, 2.0L);
    Ray ray(origin, target);

    EXPECT_DOUBLE_EQ(ray.origin().x, 0.0L);
    EXPECT_DOUBLE_EQ(ray.origin().y, 0.0L);
    EXPECT_DOUBLE_EQ(ray.direction().z, 1.0L); // pointing towards +z
}