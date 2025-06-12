#ifndef PRISM_SPHERE_HPP_
#define PRISM_SPHERE_HPP_

#include "prism_export.h"
#include "Prism/objects.hpp"
#include "Prism/point.hpp"
#include "Prism/vector.hpp"

namespace Prism {

class PRISM_EXPORT Sphere : public Object {
  public:
    Sphere(Point3 center, ld radius, Material* material);

    virtual bool hit(const Ray& ray, ld t_min, ld t_max, HitRecord& rec) const override;

  private:
    Point3 center;
    ld radius;
    Material* material;
};

} // namespace Prism

#endif // PRISM_SPHERE_HPP_