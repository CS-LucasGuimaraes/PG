#include "Prism/scene/octree.hpp"

#include "Prism/core/style.hpp"
#include "Prism/objects/mesh.hpp"

namespace Prism {

std::vector<AABB> subdivide_bounds(const AABB& parent_box) {
    std::vector<AABB> children;
    children.reserve(8);

    const Point3& min = parent_box.min;
    const Point3& max = parent_box.max;
    Point3 center = min + (max - min) * 0.5;

    children.emplace_back(AABB(min, center));
    children.emplace_back(AABB(Point3(center.x, min.y, min.z), Point3(max.x, center.y, center.z)));
    children.emplace_back(AABB(Point3(min.x, center.y, min.z), Point3(center.x, max.y, center.z)));
    children.emplace_back(AABB(Point3(center.x, center.y, min.z), Point3(max.x, max.y, center.z)));

    children.emplace_back(AABB(Point3(min.x, min.y, center.z), Point3(center.x, center.y, max.z)));
    children.emplace_back(AABB(Point3(center.x, min.y, center.z), Point3(max.x, center.y, max.z)));
    children.emplace_back(AABB(Point3(min.x, center.y, center.z), Point3(center.x, max.y, max.z)));
    children.emplace_back(AABB(center, max));

    return children;
}

bool boxes_overlap(const AABB& box1, const AABB& box2) {
    bool overlap_x = (box1.min.x <= box2.max.x) && (box1.max.x >= box2.min.x);
    bool overlap_y = (box1.min.y <= box2.max.y) && (box1.max.y >= box2.min.y);
    bool overlap_z = (box1.min.z <= box2.max.z) && (box1.max.z >= box2.min.z);
    return overlap_x && overlap_y && overlap_z;
}

AABB surrounding_box(const AABB& box0, const AABB& box1) {
    Point3 small(std::min(box0.min.x, box1.min.x), std::min(box0.min.y, box1.min.y),
                 std::min(box0.min.z, box1.min.z));

    Point3 big(std::max(box0.max.x, box1.max.x), std::max(box0.max.y, box1.max.y),
               std::max(box0.max.z, box1.max.z));

    return AABB(small, big);
}

Octree::Octree(const std::vector<Object*>& scene_objects) {
    if (scene_objects.empty()) {
        Style::logWarning("Octree constructor called with zero objects.");
        return;
    }

    std::vector<Object*> finite_objects;

    for (Object* obj : scene_objects) {
        if (obj->get_bounding_box().is_finite()) {
            if (auto mesh = dynamic_cast<Mesh*>(obj)) {
                for (const auto& tri : mesh->getMesh()) {
                    auto triangle_ptr = std::make_unique<Triangle>(
                        tri.getPoint1(),
                        tri.getPoint2(),
                        tri.getPoint3(),
                        mesh->getMaterial()
                    );
                    triangle_ptr->setTransform(mesh->getTransform());
                    finite_objects.push_back(triangle_ptr.get());
                    new_objects.emplace_back(std::move(triangle_ptr));
                }
            }
            else finite_objects.push_back(obj);
        }
        else {
            infinite_objects.push_back(obj);
        }
    }


    AABB scene_box = finite_objects[0]->get_bounding_box();
    for (size_t i = 1; i < finite_objects.size(); ++i) {
        scene_box = surrounding_box(scene_box, finite_objects[i]->get_bounding_box());
    }

    // 2. Cria o nó raiz e inicia a construção recursiva
    root = std::make_unique<OctreeNode>(scene_box);
    build(root.get(), finite_objects, 0);
}

const int MAX_DEPTH = 8;
const int MAX_OBJECTS_PER_NODE = 5;

void Octree::build(OctreeNode* node, const std::vector<Object*>& node_objects, int depth) {
    if (depth >= MAX_DEPTH || node_objects.size() <= MAX_OBJECTS_PER_NODE) {
        node->objects = node_objects;
        return;
    }

    std::vector<AABB> child_bounds = subdivide_bounds(node->bounds);
    for (const auto& bound : child_bounds) {
        node->children.emplace_back(std::make_unique<OctreeNode>(bound));
    }

    for (Object* obj : node_objects) {
        AABB obj_box = obj->get_bounding_box();

        for (int i = 0; i < 8; ++i) {
            if (boxes_overlap(obj_box, node->children[i]->bounds)) {
                node->children[i]->objects.push_back(obj);
            }
        }
    }

    for (int i = 0; i < 8; ++i) {
        if (!node->children[i]->objects.empty()) {
            build(node->children[i].get(), node->children[i]->objects, depth + 1);
        }
    }

    node->objects.clear();
    node->objects.shrink_to_fit();
}

bool Octree::hit_closest(const Ray& ray, double t_min, double t_max, HitRecord& rec) const {
    bool hit_anything = false;
    rec.t = t_max;

    if (hit_closest_recursive(root.get(), ray, t_min, rec.t, rec)) {
        hit_anything = true;
    }

    for (const auto& infinite_obj : infinite_objects) {
        if (infinite_obj->hit(ray, t_min, rec.t, rec)) {
            hit_anything = true;
        }
    }

    return hit_anything;
}

bool Octree::hit_closest_recursive(const OctreeNode* node, const Ray& ray, double t_min,
                                   double t_max, HitRecord& rec) const {

    if (!node->bounds.hit(ray, t_min, t_max)) {
        return false;
    }

    bool hit_anything = false;
    double closest_so_far = t_max;

    if (node->is_leaf()) {
        for (const auto& object : node->objects) {
            if (object->hit(ray, t_min, closest_so_far, rec)) {
                hit_anything = true;
                closest_so_far = rec.t;
            }
        }
    }

    else {
        for (const auto& child : node->children) {
            if (hit_closest_recursive(child.get(), ray, t_min, closest_so_far, rec)) {
                hit_anything = true;
                closest_so_far = rec.t;
            }
        }
    }
    return hit_anything;
}

bool Octree::hit_any(const Ray& ray, double t_min, double t_max, HitRecord& rec) const {
    if (hit_any_recursive(root.get(), ray, t_min, t_max, rec)) {
        return true;
    }

    for (const auto& infinite_obj : infinite_objects) {
        if (infinite_obj->hit(ray, t_min, t_max, rec)) {
            return true;
        }
    }

    return false;
}

bool Octree::hit_any_recursive(const OctreeNode* node, const Ray& ray, double t_min, double t_max,
                               HitRecord& rec) const {

    if (!node->bounds.hit(ray, t_min, t_max)) {
        return false;
    }

    if (node->is_leaf()) {
        for (const auto& object : node->objects) {
            if (object->hit(ray, t_min, t_max, rec)) {
                return true;
            }
        }
    } else {
        for (const auto& child : node->children) {
            if (hit_any_recursive(child.get(), ray, t_min, t_max, rec)) {
                return true;
            }
        }
    }

    return false;
}

} // namespace Prism