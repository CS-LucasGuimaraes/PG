#include "Prism/objects/mesh.hpp"

#include "Prism/core/matrix.hpp"

#include <cmath>

namespace Prism {
// TODO: Implementar BVH aqui

Mesh::Mesh(std::filesystem::path& path) {
    ObjReader reader(path.string());
    material = std::move(reader.curMaterial);

    for (auto& point : reader.vertices) {
        points.emplace_back(std::make_shared<Point3>(point[0], point[1], point[2]));
    }

    for (auto& normal : reader.normals) {
        normals.emplace_back(std::make_shared<Vector3>(normal[0], normal[1], normal[2]));
    }

    for (auto& face : reader.faces) {
        mesh.emplace_back(points[face.vertex_indices[0]], points[face.vertex_indices[1]],
                          points[face.vertex_indices[2]], normals[face.normal_indices[0]],
                          normals[face.normal_indices[1]], normals[face.normal_indices[2]]);
    }
};

Mesh::Mesh(ObjReader& reader) : material(std::move(reader.curMaterial)) {

    for (auto& point : reader.vertices) {
        points.emplace_back(std::make_shared<Point3>(point[0], point[1], point[2]));
    }

    for (auto& normal : reader.normals) {
        normals.emplace_back(std::make_shared<Vector3>(normal[0], normal[1], normal[2]));
    }

    for (auto& face : reader.faces) {
        mesh.emplace_back(points[face.vertex_indices[0]], points[face.vertex_indices[1]],
                          points[face.vertex_indices[2]], normals[face.normal_indices[0]],
                          normals[face.normal_indices[1]], normals[face.normal_indices[2]]);
    }
};

bool Mesh::hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const {
    Ray transformed_ray = ray.transform(inverseTransform);

    rec.t = t_max;
    for (const auto& triangle : mesh) {
        triangle.hit(transformed_ray, t_min, rec.t, rec);
    }

    Point3 world_p;
    double world_t = INFINITY;

    if (rec.t < t_max) {
        world_p = transform * transformed_ray.at(rec.t);
        world_t = (world_p - ray.origin()).dot(ray.direction().normalize());
    }

    if (world_t < t_max) {
        rec.p = world_p;
        rec.t = world_t;
        Vector3 world_normal = (inverseTransposeTransform * rec.normal).normalize();
        rec.set_face_normal(ray, world_normal);
        rec.material = material;
        return true;
    }

    return false;
};

void Mesh::setMaterial(std::shared_ptr<Material> new_material) {
    material = std::move(new_material);
}

}; // namespace Prism
