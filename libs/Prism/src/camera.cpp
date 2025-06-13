#include "Prism/camera.hpp"
#include "Prism/matrix.hpp"
#include "Prism/point.hpp"
#include "Prism/ray.hpp"
#include "Prism/utils.hpp"
#include "Prism/vector.hpp"
#include <cmath>
#include <stdexcept>

namespace Prism {

Camera::Camera(const Point3& position, const Point3& target, const Vector3& upvec,
               const double& distance, const double& viewport_height, const double& viewport_width,
               int image_height, int image_width)
    : screen_distance(distance), screen_height(viewport_height), screen_width(viewport_width),
      pixel_height(image_height), pixel_width(image_width) {
    pos = new Point3(position);
    aim = new Point3(target);
    up = new Vector3(upvec);
    coordinate_basis = new Matrix<double>;
    *coordinate_basis = orthonormalBasisContaining(*pos - *aim);

    // Precalc pixel locations and deltas
    const auto& basis = *coordinate_basis;
    Vector3 w = Vector3{basis[0][0], basis[1][0], basis[2][0]};
    Vector3 u = Vector3{basis[0][1], basis[1][1], basis[2][1]};
    Vector3 v = Vector3{basis[0][2], basis[1][2], basis[2][2]};

    Point3 screen_center = *pos - (w * screen_distance);
    Point3 top_left_corner =
        screen_center - (u * (screen_width / 2.0)) + (v * (screen_height / 2.0));

    // O cálculo agora mistura 'double' e 'int' corretamente
    pixel_delta_u = new Vector3(u * (screen_width / pixel_width));
    pixel_delta_v = new Vector3(v * (screen_height / pixel_height));

    pixel_00_loc = new Point3(top_left_corner + (*pixel_delta_u * 0.5) - (*pixel_delta_v * 0.5));
}

Camera::~Camera() {
    delete pos;
    delete aim;
    delete up;
    delete coordinate_basis;
    delete pixel_00_loc;
    delete pixel_delta_u;
    delete pixel_delta_v;
}

} // namespace Prism