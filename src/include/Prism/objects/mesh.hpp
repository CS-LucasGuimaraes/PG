#ifndef PRISM_MESH_HPP_
#define PRISM_MESH_HPP_

#include "Prism/ObjReader.hpp"
#include "Prism/material.hpp"
#include "Prism/objects.hpp"
#include "Prism/point.hpp"
#include "Prism/triangle.hpp"
#include "prism_export.h"
#include <array>
#include <memory>
#include <vector>

namespace Prism {

/**
 * @class Mesh
 * @brief Represents a 3D mesh object composed of triangles.
 * The Mesh class is designed to hold a collection of triangles, each defined by three points in 3D
 * space. It provides functionality to read mesh data from an OBJ file and check for ray
 * intersections with the mesh. It inherits from the Object class, allowing it to be used in a scene
 * with other objects.
 */
class PRISM_EXPORT Mesh : public Object {
  public:
    /**
     * @brief Constructs a Mesh object from a file path.
     * @param path The file path to the OBJ file containing the mesh data.
     * This constructor initializes the Mesh by reading points and triangles from the specified OBJ
     * file.
     */
    explicit Mesh(std::string path);

    /**
     * @brief Constructs a Mesh object from an ObjReader.
     * @param reader An ObjReader object that contains the mesh data to be loaded.
     * This constructor initializes the Mesh by reading points and triangles from the provided
     * ObjReader.
     */
    explicit Mesh(ObjReader& reader);

    /**
     * @brief Checks if a ray intersects with the mesh.
     * @param ray The ray to test for intersection with the mesh.
     * @param t_min The minimum distance for a valid hit.
     * @param t_max The maximum distance for a valid hit.
     * @param rec The hit record to be filled with intersection details if a hit occurs.
     * @return True if the ray intersects with the mesh within the specified distance range, false
     * otherwise. This method transforms the ray using the inverse transformation matrix of the mesh
     * and checks each triangle for intersection. If a hit is found, it updates the hit record with
     * the intersection point, normal, and material properties.
     */
    virtual bool hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const override;

  private:
    std::vector<std::shared_ptr<Point3>> points; ///< Points that define the vertices of the mesh
    std::vector<MeshTriangle>
        mesh; ///< Triangles that make up the mesh, each defined by three points
    std::shared_ptr<Material>
        material; ///< Material properties of the mesh, defining how it interacts with light
};

} // namespace Prism

#endif // PRISM_MESH_HPP_