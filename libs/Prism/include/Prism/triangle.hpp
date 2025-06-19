#ifndef PRISM_TRIANGLE_HPP_
#define PRISM_TRIANGLE_HPP_

#include "prism_export.h"
#include "Prism/objects.hpp"
#include "Prism/point.hpp"
#include "Prism/vector.hpp"

namespace Prism {

class PRISM_EXPORT Triangle : public Object {
  public:
    Triangle(Point3 point1 = Point3(), Point3 point2 = Point3(), Point3 point3 = Point3(), std ::shared_ptr<Material> material = std::make_shared<Prism::Material>());

    virtual bool hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const override;
    std::shared_ptr<Material> material;
    Point3 point1;
    Point3 point2;
    Point3 point3;

  private:
};

} // namespace Prism

#endif // PRISM_TRIANGLE_HPP_