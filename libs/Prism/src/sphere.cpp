#include "Prism/Sphere.hpp"
#include "Prism/Ray.hpp" 
#include "Prism/utils.hpp"
#include <cmath>

namespace Prism {

Sphere::Sphere(Point3 center, ld radius, Material* material)
    : center(center), radius(radius), material(material) {}

//(d⋅d)t2+(2d⋅oc)t+(oc⋅oc−r2)=0
//solve using reducted quadratic formula
// t = (-b ± √(b² - 4ac)) / 2a
// b' = b/2  =>  b = 2b'
// t = (-2b' ± √(4b'² - 4ac)) / 2a
// t = (-2b' ± 2√(b'² - ac)) / 2a
// t = -b' ± √(b'² - ac) / a
bool Sphere::hit(const Ray& ray, ld t_min, ld t_max, HitRecord& rec) const {
    Vector3 oc = ray.origin() - center;
    auto a = sqr(ray.direction().magnitude());
    auto halfb = oc * ray.direction();
    auto c = sqr(oc.magnitude) - sqr(radius);

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
    rec.p = ray.at(rec.t);
    Vector3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(ray, outward_normal);
    rec.material = material;

    return true;
}

} // namespace Prism