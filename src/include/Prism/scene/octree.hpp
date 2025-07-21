#ifndef PRISM_OCTREE_HPP_
#define PRISM_OCTREE_HPP_

#include "prism_export.h"

#include "Prism/objects/aabb.hpp"
#include "Prism/objects/objects.hpp"
#include "Prism/scene/acceleration.hpp"
#include "Prism/objects/triangle.hpp"

#include <memory>
#include <vector>

namespace Prism {

class OctreeNode {
  public:
    AABB bounds;
    std::vector<std::unique_ptr<OctreeNode>> children;
    std::vector<Object*> objects;

    OctreeNode() = default;
    OctreeNode(const AABB& b) : bounds(b) {
    }

    bool is_leaf() const {
        return children.empty();
    }
};

class PRISM_EXPORT Octree : public AccelerationStructure {
  public:
  
  Octree(const std::vector<Object*>& scene_objects);
  
  
  virtual bool hit_closest(const Ray& ray, double t_min, double t_max, HitRecord& rec) const;
  virtual bool hit_any(const Ray& ray, double t_min, double t_max, HitRecord& rec) const;
  
  private:
    std::unique_ptr<OctreeNode> root;
    std::vector<Object*> infinite_objects;
    std::vector<std::unique_ptr<Triangle>> new_objects;

    void build(OctreeNode* node, const std::vector<Object*>& node_objects, int depth = 0);
  
    bool hit_closest_recursive(const OctreeNode* node, const Ray& ray, double t_min, double t_max,
                       HitRecord& rec) const;

    bool hit_any_recursive(const OctreeNode* node, const Ray& ray, double t_min, double t_max,
                   HitRecord& rec) const;
};

} // namespace Prism

#endif // PRISM_OCTREE_HPP_