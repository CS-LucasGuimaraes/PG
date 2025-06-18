#include "Prism/triangle.hpp"
#include "Prism/ray.hpp"
#include "Prism/utils.hpp"
#include <cmath>

//r: O + tv
//Δ: p1, p2, p3
//π: p1 , n = (p2-p1)x(p3-p1)
//|   .    .          .    |  |t|   
//|  -v   (p2-p1)  (p3-p1) |* |a| = O - p1
//|   .    .          .    |  |b|    
namespace Prism {

Triangle::Triangle(Point3 point1, Point3 point2, Point3 point3,std::shared_ptr<Material> material)
: point1(point1),point2(point2),point3(point3),material(std::move(material)) {}
 
bool Triangle::hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const {

    constexpr double epsilon = std::numeric_limits<double>::epsilon();
    const Vector3 ray_direction = ray.direction();

    // step 1: calculate the vectors that form the triangle
    const Vector3 vector1 = point2 - point1;
    const Vector3 vector2 = point3 - point1;
   
    // step 2: calculate the determinant of the matrix
    const Vector3 rd_cross_v2 = ray_direction^vector2;
    const double det = vector1*rd_cross_v2;

    // if the determinant is zero, the plane is parallel to the ray
    if(det > -epsilon && det < epsilon) return false;

    // step 3: calculate ‘a’ using Cramer's rule
    const double det_inverse = 1.0/det;
    const Vector3 result = ray.origin() - point1;
    const double a = (result*rd_cross_v2)*det_inverse;

    // ‘a’ negative or greater than 1: point is outside the triangle
    if((a < 0 && std::abs(a) > epsilon) || (a > 1 && std::abs(a-1) > epsilon)) return false;

    // step 4: calculate ‘b’ using Cramer's rule
    const Vector3 res_cross_v1 = result^vector1;
    const double b = (ray_direction*res_cross_v1)*det_inverse;

    // ‘b’ negative or a+b > 1(c negative): point is outside the triangle 
    if((b < 0 && std::abs(b) > epsilon) || (a + b > 1 && std::abs(a + b - 1) > epsilon)) return false;

    const double t = (vector2*res_cross_v1)*det_inverse;
    
    // point is inside the triangle
    if(t>epsilon && t > t_min && t < t_max){
        rec.t = t;
        rec.p = ray.at(rec.t);
        rec.set_face_normal(ray,vector1^vector2);
        rec.material = material;
        return true;
    }
    //ponto está fora do alcance
    return false;
}

}



