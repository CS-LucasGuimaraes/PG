#include "Prism/mesh.hpp"
#include <cmath>

namespace Prism {
// TODO: Implementar BVH aqui

Mesh::Mesh(ObjReader& reader):material(std::move(reader.curMaterial)){

    for(auto& point: reader.vertices){
        points.emplace_back(std::make_shared<Point3>(point[0],point[1],point[2]));
    }
    for(auto& triangle: reader.triangles){
        mesh.push_back(
            Triangle<std::shared_ptr<Point3>>(
            points[triangle[0]],
            points[triangle[1]],
            points[triangle[2]],
            material
        )
        );
    }
};

bool Mesh::hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const {
    Ray transformed_ray = ray.transform(inverseTransform);

    bool hit_anything = false;
    rec.t = INFINITY;
    for (const auto& triangle : mesh) {
        if (triangle.hit_in_mesh(ray, 0.001, rec.t, rec, transform, inverseTransform, inverseTransposeTransform)) {
            hit_anything = true;
        }
    }
    return hit_anything;
};

};
