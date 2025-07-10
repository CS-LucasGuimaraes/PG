#include "Prism/objects/mesh.hpp"

#include "Prism/core/matrix.hpp"

#include <cmath>

namespace Prism {
// TODO: Implementar BVH aqui

Mesh::Mesh(std::string path) {
    ObjReader reader(path);
    material = std::move(reader.curMaterial);

    for (auto& point : reader.vertices) {
        points.emplace_back(std::make_shared<Point3>(point[0], point[1], point[2]));
    }
    for (auto& triangle : reader.triangles) {
        mesh.push_back({points[triangle[0]], points[triangle[1]], points[triangle[2]]});
    }
};

Mesh::Mesh(ObjReader& reader) : material(std::move(reader.curMaterial)) {

    for (auto& point : reader.vertices) {
        points.emplace_back(std::make_shared<Point3>(point[0], point[1], point[2]));
    }
    for (auto& triangle : reader.triangles) {
        mesh.push_back({points[triangle[0]], points[triangle[1]], points[triangle[2]]});
    }
};

bool Mesh::hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const {
    Ray transformed_ray = ray.transform(inverseTransform);

    rec.t = t_max;
    for (const auto& triangle : mesh) {
        triangle.hit(transformed_ray, 0.001, rec.t, rec);
    }

    if (rec.t < t_max) {
        rec.p = transform * transformed_ray.at(rec.t);
        Vector3 world_normal = (inverseTransposeTransform * rec.normal).normalize();
        rec.set_face_normal(ray, world_normal);
        rec.material = material;
        return true;
    }

    return false;
};

}; // namespace Prism
