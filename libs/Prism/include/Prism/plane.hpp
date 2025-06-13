#ifndef PRISM_PLANE_HPP_
#define PRISM_PLANE_HPP_

#include "prism_export.h"
#include "Prism/objects.hpp"
#include "Prism/point.hpp"
#include "Prism/vector.hpp"

namespace Prism {

class PRISM_EXPORT Plane : public Object {
  public:
    Plane(Point3 point_on_plane, Vector3 normal, Material* material);

    virtual bool hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const override;

  private:
    Point3 point_on_plane;
    Vector3 normal;
    Material* material;
};

} // namespace Prism

#endif // PRISM_PLANE_HPP_