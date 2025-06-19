#include "Prism/triangle.hpp"
#include "Prism/ray.hpp"
#include "Prism/utils.hpp"
#include "Prism/material.hpp"
#include <cmath>

//r: O + tv
//Δ: p1, p2, p3
//π: p1 , n = (p2-p1)x(p3-p1)
//|   .    .          .    |  |t|   
//|  -v   (p2-p1)  (p3-p1) |* |a| = O - p1
//|   .    .          .    |  |b|    
namespace Prism {

Triangle::Triangle(Point3 point1, Point3 point2, Point3 point3,std::shared_ptr<Material> material)
: point1(point1),point2(point2),point3(point3),material(std::move(material)) {}
 
bool Triangle::hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const {
    // Passo 1: Criar um raio transformado.
    Ray transformed_ray = ray.transform(inverseTransform);
    
    // Passo 2: Realizar o teste de colisão em espaço local (algoritmo Möller-Trumbore).
    const double epsilon = std::numeric_limits<double>::epsilon();
    const Vector3 ray_direction = transformed_ray.direction();

    const Vector3 edge1 = point2 - point1;
    const Vector3 edge2 = point3 - point1;
   
    const Vector3 h = ray_direction ^ edge2;
    const double a = edge1 * h;

    if (a > -epsilon && a < epsilon)
        return false; // Raio paralelo ao triângulo.

    const double f = 1.0 / a;
    const Vector3 s = transformed_ray.origin() - point1;
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

}



