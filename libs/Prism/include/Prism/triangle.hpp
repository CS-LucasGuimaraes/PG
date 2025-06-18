#ifndef PRISM_TRIANGLE_HPP_
#define PRISM_TRIANGLE_HPP_

#include "prism_export.h"
#include "Prism/objects.hpp"
#include "Prism/point.hpp"
#include "Prism/vector.hpp"

namespace Prism {

class PRISM_EXPORT Triangle : public Object {
  public:
    Triangle(Point3 point1, Point3 point2, Point3 point3,std::shared_ptr<Material> material);

    virtual bool hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const override;

  private:
    Point3 point1;
    Point3 point2;
    Point3 point3;
    std::shared_ptr<Material> material;
};

} // namespace Prism

#endif // PRISM_TRIANGLE_HPP_