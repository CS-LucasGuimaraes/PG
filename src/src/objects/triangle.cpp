#include "Prism/objects/triangle.hpp"

#include "Prism/core/matrix.hpp"

#include <cmath>

namespace Prism {

Triangle::Triangle(Point3 p1, Point3 p2, Point3 p3, std::shared_ptr<Material> mat)
    : point1(p1), point2(p2), point3(p3), material(std::move(mat)) {
}

Point3 Triangle::getPoint1() const {
    return point1;
}
Point3 Triangle::getPoint2() const {
    return point2;
}
Point3 Triangle::getPoint3() const {
    return point3;
}

bool Triangle::hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const {
    const Ray transformed_ray = ray.transform(inverseTransform);

    const double epsilon = 1e-8;
    const Vector3 edge1 = this->getPoint2() - this->getPoint1();
    const Vector3 edge2 = this->getPoint3() - this->getPoint1();

    const Vector3 h = transformed_ray.direction().cross(edge2);
    const double a = edge1.dot(h);

    if (a > -epsilon && a < epsilon) {
        return false;
    }

    const double f = 1.0 / a;
    const Vector3 s = transformed_ray.origin() - this->getPoint1();
    const double u = f * s.dot(h);

    if (u < 0.0 || u > 1.0) {
        return false;
    }

    const Vector3 q = s.cross(edge1);
    const double v = f * (transformed_ray.direction() * q);

    if (v < 0.0 || u + v > 1.0) {
        return false;
    }

    const double t_local = f * edge2.dot(q);
    Point3 world_hit_point = transform * transformed_ray.at(t_local);
    const double t_global = (world_hit_point - ray.origin()).dot(ray.direction());

    if (t_global < t_min || t_global > t_max) {
        return false;
    }

    rec.t = t_global;
    rec.p = world_hit_point;

    Vector3 local_normal = edge1.cross(edge2);
    Vector3 world_normal = (inverseTransposeTransform * local_normal).normalize();
    rec.set_face_normal(ray, world_normal);

    rec.material = material;

    return true;
}

MeshTriangle::MeshTriangle(std::shared_ptr<Point3> p1, std::shared_ptr<Point3> p2,
                           std::shared_ptr<Point3> p3, std::shared_ptr<Vector3> n1,
                           std::shared_ptr<Vector3> n2, std::shared_ptr<Vector3> n3)
    : point1(std::move(p1)), point2(std::move(p2)), point3(std::move(p3)), normal1(std::move(n1)),
      normal2(std::move(n2)), normal3(std::move(n3)) {
}

MeshTriangle::MeshTriangle(Point3 p1, Point3 p2, Point3 p3)
    : point1(std::make_shared<Point3>(p1)), point2(std::make_shared<Point3>(p2)),
      point3(std::make_shared<Point3>(p3)) {
}

MeshTriangle::MeshTriangle(std::initializer_list<Point3> points) {
    if (points.size() != 3) {
        throw std::invalid_argument("MeshTriangle requires exactly three points.");
    }
    auto it = points.begin();
    point1 = std::make_shared<Point3>(*it++);
    point2 = std::make_shared<Point3>(*it++);
    point3 = std::make_shared<Point3>(*it);
}

Point3 MeshTriangle::getPoint1() const {
    return *point1;
}
Point3 MeshTriangle::getPoint2() const {
    return *point2;
}
Point3 MeshTriangle::getPoint3() const {
    return *point3;
}

bool MeshTriangle::hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const {

    const double epsilon = 1e-8;
    const Vector3 ray_direction = ray.direction();
    const Vector3 edge1 = this->getPoint2() - this->getPoint1();
    const Vector3 edge2 = this->getPoint3() - this->getPoint1();
    const Vector3 h = ray_direction ^ edge2;
    const double a = edge1 * h;

    if (a > -epsilon && a < epsilon)
        return false;

    const double f = 1.0 / a;
    const Vector3 s = ray.origin() - this->getPoint1();
    const double u = f * (s * h);

    if (u < 0.0 || u > 1.0)
        return false;

    const Vector3 q = s ^ edge1;
    const double v = f * (ray_direction * q);

    if (v < 0.0 || u + v > 1.0)
        return false;

    const double t = f * (edge2 * q);

    if (t > t_min && t < t_max) {

        const double w = 1.0 - u - v;
        rec.t = t;
        rec.normal = ((*normal1 * w) + (*normal2 * u) + (*normal3 * v)).normalize();
        return true;
    }
    return false;
}

} // namespace Prism