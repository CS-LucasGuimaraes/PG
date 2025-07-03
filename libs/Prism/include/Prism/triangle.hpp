#ifndef PRISM_TRIANGLE_HPP_
#define PRISM_TRIANGLE_HPP_

#include "prism_export.h"
#include "Prism/objects.hpp"
#include "Prism/point.hpp"
#include "Prism/vector.hpp"
#include "Prism/ray.hpp"
#include "Prism/utils.hpp"
#include "Prism/material.hpp"
#include <cmath>

namespace Prism {

template <typename Point> class PRISM_EXPORT Triangle : public Object {
  static_assert(std::is_same<Point, Point3>::value || std::is_same<Point, Point3&>::value || std::is_same<Point, std::shared_ptr<Point3>>::value, "T must be Point3 , Point3& or std::shared_ptr<Point3>");

  public:
    Triangle(Point point1, Point point2, Point point3, std ::shared_ptr<Material> material = std::make_shared<Prism::Material>()): 
    point1(point1),point2(point2),point3(point3),material(std::move(material)){};

    Point3 getPoint1() const{
        Point3 point;
        if constexpr (std::is_same<Point, std::shared_ptr<Point3>>::value) point = *point1;
        else point = point1;
        return point;
    };

    Point3 getPoint2() const{
        Point3 point;
        if constexpr(std::is_same<Point, std::shared_ptr<Point3>>::value) point = *point2;
        else point = point2;
        return point;
    };

    Point3 getPoint3() const{
        Point3 point;
        if constexpr (std::is_same<Point, std::shared_ptr<Point3>>::value) point = *point3;
        else point = point3;
        return point;
    };

    virtual bool hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const override;

    std::shared_ptr<Material> material;
  private:
    Point point1;
    Point point2;
    Point point3;
};
 
template <typename Point> bool Triangle<Point>::hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const {
    // Passo 1: Criar um raio transformado.
    Ray transformed_ray = ray.transform(inverseTransform);
    
    // Passo 2: Realizar o teste de colisão em espaço local (algoritmo Möller-Trumbore).
    const double epsilon = std::numeric_limits<double>::epsilon();
    const Vector3 ray_direction = transformed_ray.direction();

    const Vector3 edge1 = this->getPoint2()- this->getPoint1();
    const Vector3 edge2 = this->getPoint3() - this->getPoint1();
    
    const Vector3 h = ray_direction ^ edge2;
    const double a = edge1 * h;

    if (a > -epsilon && a < epsilon)
        return false; // Raio paralelo ao triângulo.

    const double f = 1.0 / a;
    const Vector3 s = transformed_ray.origin() - this->getPoint1();
    const double u = f * (s * h);

    if (u < 0.0 || u > 1.0)
        return false;

    const Vector3 q = s ^ edge1;
    const double v = f * (ray_direction * q);

    if (v < 0.0 || u + v > 1.0)
        return false;

    const double t = f * (edge2 * q);

    if (t > t_min && t < t_max) {
        rec.t = t;
        rec.p = transform * transformed_ray.at(t);
        Vector3 local_normal = edge1 ^ edge2;
        Vector3 world_normal = (inverseTransposeTransform * local_normal).normalize();
        rec.set_face_normal(ray, world_normal);
        rec.material = material;
        return true;
    }

    return false;
}

} // namespace Prism

#endif // PRISM_TRIANGLE_HPP_