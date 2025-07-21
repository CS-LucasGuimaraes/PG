/**
 * @file triangle.hpp
 * @brief Prism Render Engine
 *
 * @copyright Copyright (c) 2025 src-lua
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef PRISM_TRIANGLE_HPP_
#define PRISM_TRIANGLE_HPP_

#include "prism_export.h"

#include "Prism/core/material.hpp"
#include "Prism/core/point.hpp"
#include "Prism/core/ray.hpp"
#include "Prism/objects/objects.hpp"

#include <initializer_list>
#include <memory>

namespace Prism {
class Matrix;     // Forward declaration for Matrix
class Ray;        // Forward declaration for Ray
class Point3;     // Forward declaration for Point3
struct HitRecord; // Forward declaration for HitRecord

/**
 * @class Triangle
 * @brief Represents a triangle in 3D space defined by its three vertices.
 * This class inherits from Object and implements the hit method to check for ray-triangle
 * intersections. It is used to represent a triangle in a 3D scene, allowing for ray tracing and
 * collision detection.
 */
class PRISM_EXPORT Triangle : public Object {
  public:
    /**
     * @brief Constructs a Triangle given its three vertices and an optional material.
     * @param p1 The first vertex of the triangle.
     * @param p2 The second vertex of the triangle.
     * @param p3 The third vertex of the triangle.
     * @param mat An optional shared pointer to a Material object associated with the triangle.
     * If no material is provided, a default Material is created.
     * This constructor initializes the triangle with the specified vertices and material
     * properties.
     */
    Triangle(Point3 p1, Point3 p2, Point3 p3,
             std::shared_ptr<Material> mat = std::make_shared<Prism::Material>());

    /**
     * @brief Gets the first vertex of the triangle.
     * @return The first vertex of the triangle as a Point3 object.
     */
    Point3 getPoint1() const;

    /**
     * @brief Gets the second vertex of the triangle.
     * @return The second vertex of the triangle as a Point3 object.
     */
    Point3 getPoint2() const;

    /**
     * @brief Gets the third vertex of the triangle.
     * @return The third vertex of the triangle as a Point3 object.
     */
    Point3 getPoint3() const;

    /**
     * @brief Checks if a ray intersects with the triangle.
     * @param ray The Ray to test for intersection with the triangle.
     * @param t_min The minimum distance for a valid hit.
     * @param t_max The maximum distance for a valid hit.
     * @param rec The HitRecord to be filled upon a collision.
     * @return True if the ray intersects the triangle within the specified distance range, false
     * otherwise. This method transforms the ray using the inverse transformation matrix of the
     * triangle and checks for intersection. If a hit is found, it updates the hit record with the
     * intersection point, normal, and material properties.
     */
    virtual bool hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const override;
 
    virtual AABB get_bounding_box() const override;

  private:
    Point3 point1; ///< The first vertex of the triangle
    Point3 point2; ///< The second vertex of the triangle
    Point3 point3; ///< The third vertex of the triangle
    std::shared_ptr<Material> material; ///< Material properties of the triangle
};

/**
 * @class MeshTriangle
 * @brief Represents a triangle in a mesh, defined by three points.
 * This class is used to represent a triangle in a 3D mesh, allowing for ray tracing and collision
 * detection. It can be constructed from shared pointers to Point3 objects or directly from Point3
 * objects.
 */
class PRISM_EXPORT MeshTriangle {
  public:
    /**
     * @brief Constructs a MeshTriangle given three points.
     * @param p1 The first point of the triangle.
     * @param p2 The second point of the triangle.
     * @param p3 The third point of the triangle.
     * @param n1 The normal vector at the first point.
     * @param n2 The normal vector at the second point.
     * @param n3 The normal vector at the third point.
     * This constructor initializes the MeshTriangle with the specified points.
     */
    MeshTriangle(std::shared_ptr<Point3> p1, std::shared_ptr<Point3> p2, std::shared_ptr<Point3> p3,
                 std::shared_ptr<Vector3> n1, std::shared_ptr<Vector3> n2,
                 std::shared_ptr<Vector3> n3);

    /**
     * @brief Constructs a MeshTriangle given three Point3 objects.
     * @param p1 The first vertex of the triangle.
     * @param p2 The second vertex of the triangle.
     * @param p3 The third vertex of the triangle.
     * This constructor initializes the MeshTriangle with the specified Point3 objects.
     */
    MeshTriangle(Point3 p1, Point3 p2, Point3 p3);

    /**
     * @brief Constructs a MeshTriangle from an initializer list of Point3 objects.
     * @param points An initializer list containing three Point3 objects representing the vertices
     * of the triangle. This constructor initializes the MeshTriangle with the specified points.
     */
    MeshTriangle(std::initializer_list<Point3> points);

    /**
     * @brief Gets the first point of the triangle.
     * @return The first point of the triangle as a Point3 object.
     */
    Point3 getPoint1() const;

    /**
     * @brief Gets the second point of the triangle.
     * @return The second point of the triangle as a Point3 object.
     */
    Point3 getPoint2() const;

    /**
     * @brief Gets the third point of the triangle.
     * @return The third point of the triangle as a Point3 object.
     */
    Point3 getPoint3() const;

    /**
     * @brief Checks if a ray intersects with the triangle.
     * @param ray The Ray to test for intersection with the triangle.
     * @param t_min The minimum distance for a valid hit.
     * @param t_max The maximum distance for a valid hit.
     * @param rec The HitRecord to be filled upon a collision.
     * @return True if the ray intersects the triangle within the specified distance range, false
     * otherwise. This method checks for intersection between the ray and the triangle defined by
     * the three points. If a hit is found, it updates the hit record with the intersection point,
     * normal, and other relevant details.
     */
    bool hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const;

  private:
    std::shared_ptr<Point3> point1; ///< The first point of the triangle
    std::shared_ptr<Point3> point2; ///< The second point of the triangle
    std::shared_ptr<Point3> point3; ///< The third point of the triangle

    std::shared_ptr<Vector3> normal1; ///< The normal vector at the first point
    std::shared_ptr<Vector3> normal2; ///< The normal vector at the second point
    std::shared_ptr<Vector3> normal3; ///< The normal vector at the third point
};

} // namespace Prism

#endif // PRISM_TRIANGLE_HPP_