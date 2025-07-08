#ifndef PRISM_MESH_HPP_
#define PRISM_MESH_HPP_

#include "prism_export.h"
#include "Prism/objects.hpp"
#include "Prism/point.hpp"
#include "Prism/triangle.hpp"
#include "Prism/ObjReader.hpp"
#include "Prism/material.hpp"
#include <vector>
#include <array>
#include <memory>

namespace Prism {

class PRISM_EXPORT Mesh : public Object {
  public:
    Mesh(ObjReader& reader);

    virtual bool hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const override;

  private:
    std::vector<std::shared_ptr<Point3>> points;
    std::vector<MeshTriangle> mesh;
    std::shared_ptr<Material> material;
};

} // namespace Prism

#endif // PRISM_MESH_HPP_