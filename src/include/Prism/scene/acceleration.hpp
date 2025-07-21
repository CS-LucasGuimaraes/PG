#ifndef PRISM_ACCELERATION_HPP_
#define PRISM_ACCELERATION_HPP_

#include "Prism/objects/objects.hpp"
#include <vector>
#include <memory>

namespace Prism {

enum class ACCELERATION {
    NONE,
    OCTREE,
    BSP
};

/**
 * @class AccelerationStructure
 * @brief Abstract base class for acceleration structures used in ray tracing.
 */
class AccelerationStructure {
  public:
    virtual ~AccelerationStructure() = default;

    virtual bool hit_closest(const Ray& ray, double t_min, double t_max, HitRecord& rec) const = 0;
    virtual bool hit_any(const Ray& ray, double t_min, double t_max, HitRecord& rec) const = 0;
};

class PRISM_EXPORT NoAcceleration : public AccelerationStructure {
  public:
    NoAcceleration(std::vector<Object*>& objects);

    virtual bool hit_closest(const Ray& ray, double t_min, double t_max, HitRecord& rec) const override;
    virtual bool hit_any(const Ray& ray, double t_min, double t_max, HitRecord& rec) const override;

  private:
    std::vector<Object*> objects_;
};


} // namespace Prism

#endif // PRISM_ACCELERATION_HPP_