#ifndef PRISM_TRIANGLE_HPP_
#define PRISM_TRIANGLE_HPP_

#include "prism_export.h"
#include "Prism/objects.hpp"
#include "Prism/point.hpp"
#include "Prism/material.hpp"
#include <memory>
#include <initializer_list>

namespace Prism { 
    class Matrix;       // Forward declaration for Matrix
    class Ray;          // Forward declaration for Ray
    class Point3;       // Forward declaration for Point3
    struct HitRecord;   // Forward declaration for HitRecord

class PRISM_EXPORT Triangle : public Object {
public:
    Triangle(Point3 p1, Point3 p2, Point3 p3, std::shared_ptr<Material> mat = std::make_shared<Prism::Material>());

    Point3 getPoint1() const;
    Point3 getPoint2() const;
    Point3 getPoint3() const;

    bool hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const override;

private:
    Point3 point1, point2, point3;
    std::shared_ptr<Material> material;
};

class PRISM_EXPORT MeshTriangle { 
public:
    MeshTriangle(std::shared_ptr<Point3> p1, std::shared_ptr<Point3> p2, std::shared_ptr<Point3> p3);
    MeshTriangle(Point3 p1, Point3 p2, Point3 p3);
    MeshTriangle(std::initializer_list<Point3> points);

    Point3 getPoint1() const;
    Point3 getPoint2() const;
    Point3 getPoint3() const;

    bool hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const;

private:
    std::shared_ptr<Point3> point1, point2, point3;
};

} // namespace Prism

#endif // PRISM_TRIANGLE_HPP_