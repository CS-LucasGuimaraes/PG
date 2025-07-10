#include "Prism/objects/sphere.hpp"

#include "Prism/core/matrix.hpp"
#include "Prism/core/utils.hpp"

#include <cmath>

namespace Prism {

Sphere::Sphere(Point3 center, double radius, std::shared_ptr<Material> material)
    : center(center), radius(radius), material(std::move(material)) {
}

//(d⋅d)t2+(2d⋅oc)t+(oc⋅oc−r2)=0
// solve using reducted quadratic formula
// t = (-b ± √(b² - 4ac)) / 2a
// b' = b/2  =>  b = 2b'
// t = (-2b' ± √(4b'² - 4ac)) / 2a
// t = (-2b' ± 2√(b'² - ac)) / 2a
// t = -b' ± √(b'² - ac) / a
bool Sphere::hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const {
    Ray transformed_ray = ray.transform(inverseTransform);

    Vector3 oc = transformed_ray.origin() - center;

    auto a = sqr(transformed_ray.direction().magnitude());
    auto halfb = oc * transformed_ray.direction();
    auto c = oc.dot(oc) - sqr(radius);

    auto discriminant = sqr(halfb) - a * c;
    if (discriminant < 0) {
        return false;
    }
    auto sqrtd = sqrt(discriminant);

    auto root = (-halfb - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-halfb + sqrtd) / a;
        if (root < t_min || t_max < root) {
            return false;
        }
    }

    rec.t = root;
    Point3 local_hit_point = transformed_ray.at(rec.t);
    Vector3 outward_normal_local = (local_hit_point - center) / radius;

    rec.p = transform * local_hit_point;
    Vector3 outward_normal_world = (inverseTransposeTransform * outward_normal_local).normalize();
    rec.set_face_normal(ray, outward_normal_world);
    rec.material = material;

    return true;
}

} // namespace Prism