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

    const Vector3 h = transformed_ray.direction() ^ edge2;
    const double a = edge1 * h;

    if (a > -epsilon && a < epsilon) {
        return false;
    }

    const double f = 1.0 / a;
    const Vector3 s = transformed_ray.origin() - this->getPoint1();
    const double u = f * (s * h);

    if (u < 0.0 || u > 1.0) {
        return false;
    }

    const Vector3 q = s ^ edge1;
    const double v = f * (transformed_ray.direction() * q);

    if (v < 0.0 || u + v > 1.0) {
        return false;
    }

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

MeshTriangle::MeshTriangle(std::shared_ptr<Point3> p1, std::shared_ptr<Point3> p2,
                           std::shared_ptr<Point3> p3)
    : point1(std::move(p1)), point2(std::move(p2)), point3(std::move(p3)) {
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
        rec.t = t;
        rec.normal = edge1 ^ edge2;
        return true;
    }
    return false;
}

} // namespace Prism