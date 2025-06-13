#ifndef PRISM_SPHERE_HPP_
#define PRISM_SPHERE_HPP_

#include "Prism/objects.hpp"
#include "Prism/point.hpp"
#include "Prism/vector.hpp"
#include "prism_export.h"

namespace Prism {

class PRISM_EXPORT Sphere : public Object {
  public:
    Sphere(Point3 center, double radius, std::shared_ptr<Material> material);

    virtual bool hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const override;

  private:
    Point3 center;
    double radius;
    std::shared_ptr<Material> material;
};

} // namespace Prism

#endif // PRISM_SPHERE_HPP_