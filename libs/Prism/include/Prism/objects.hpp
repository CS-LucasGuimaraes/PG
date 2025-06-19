#ifndef PRISM_OBJECT_HPP_
#define PRISM_OBJECT_HPP_

#include "Prism/matrix.hpp"
#include "Prism/point.hpp"
#include "Prism/ray.hpp"
#include "Prism/vector.hpp"
#include "prism_export.h"

namespace Prism {

class Ray;      // Forward declaration of Ray class
class Material; // Forward declaration of Material class

struct PRISM_EXPORT HitRecord {
    Point3 p;
    Vector3 normal;
    double t;
    std::shared_ptr<Material> material;
    bool front_face;

    inline void set_face_normal(const Ray& ray, const Vector3& outward_normal) {
        front_face = (ray.direction()).dot(outward_normal) < 0;
        normal = front_face ? outward_normal : outward_normal * -1;
    }
};

class PRISM_EXPORT Object {
  public:
    virtual ~Object() = default;

    /**
     * @brief Checks if a ray hits the object within a given distance range.
     * @param ray The ray to test.
     * @param t_min The minimum distance for a valid hit.
     * @param t_max The maximum distance for a valid hit.
     * @param rec The hit record to be filled upon a collision.
     * @return True if a valid hit was found, false otherwise.
     */
    virtual bool hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const = 0;

    void setTransform(const Matrix& new_transform) {
        transform = new_transform;
        inverseTransform = new_transform.inverse();
        inverseTransposeTransform = inverseTransform.transpose();
    }

  protected:
    Matrix transform = Matrix::identity(4);
    Matrix inverseTransform = Matrix::identity(4);
    Matrix inverseTransposeTransform = Matrix::identity(4);
};

} // namespace Prism

#endif // PRISM_OBJECT_HPP_