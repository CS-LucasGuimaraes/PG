#include "Prism/scene/camera.hpp"

#include "Prism/core/utils.hpp"

#include <cmath>
#include <stdexcept>

namespace Prism {

Camera::Camera(const Point3& position, const Point3& target, const Vector3& upvec, double distance,
               double viewport_height, double viewport_width, int image_height, int image_width)
    : pos(position), aim(target), up(upvec), coordinate_basis(), screen_distance(distance),
      screen_height(viewport_height), screen_width(viewport_width), pixel_height(image_height),
      pixel_width(image_width) {
    coordinate_basis = orthonormalBasisContaining(pos - aim);

    const auto& basis = coordinate_basis;
    Vector3 w = Vector3{basis[0][0], basis[1][0], basis[2][0]};
    Vector3 u = Vector3{basis[0][1], basis[1][1], basis[2][1]};
    Vector3 v = Vector3{basis[0][2], basis[1][2], basis[2][2]};

    Point3 screen_center = pos - (w * screen_distance);
    Point3 top_left_corner =
        screen_center - (u * (screen_width / 2.0)) + (v * (screen_height / 2.0));

    pixel_delta_u = u * (screen_width / pixel_width);
    pixel_delta_v = v * (screen_height / pixel_height);
    pixel_00_loc = top_left_corner + (pixel_delta_u * 0.5) - (pixel_delta_v * 0.5);
}

} // namespace Prism