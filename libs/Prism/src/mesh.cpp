#include "Prism/mesh.hpp"
#include "Prism/matrix.hpp"
#include <cmath>

namespace Prism {
// TODO: Implementar BVH aqui

Mesh::Mesh(ObjReader& reader):material(std::move(reader.curMaterial)){

    for(auto& point: reader.vertices){
        points.emplace_back(std::make_shared<Point3>(point[0],point[1],point[2]));
    }
    for(auto& triangle: reader.triangles){
        mesh.push_back({
            points[triangle[0]],
            points[triangle[1]],
            points[triangle[2]]}
        );
    }
};

bool Mesh::hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const {
    Ray transformed_ray = ray.transform(inverseTransform);

    bool hit_anything = false;
    rec.t = INFINITY;
    for (const auto& triangle : mesh) {
        if (triangle.hit(ray, 0.001, rec.t, rec)) {
            hit_anything = true;
            rec.p = transform * ray.at(rec.t);
            Vector3 world_normal = (inverseTransposeTransform * rec.normal).normalize();
            rec.set_face_normal(ray, world_normal);
            rec.material = material;
        }
    }
    return hit_anything;
};

};
