/**
 * @file camera.cpp
 * @brief Prism Render Engine
 *
 * @copyright Copyright (c) 2025 src-lua
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "Prism/scene/camera.hpp"

#include "Prism/core/utils.hpp"

#include <cmath>
#include <stdexcept>

namespace Prism {

Camera::Camera(const Point3& position, const Point3& target, const Vector3& upvec, double distance,
               double viewport_height, double viewport_width, int image_height, int image_width)
    : pos(position), aim(target), up(upvec), screen_distance(distance),
      screen_height(viewport_height), screen_width(viewport_width), pixel_height(image_height),
      pixel_width(image_width) {

    Vector3 w = (pos - aim).normalize();
    Vector3 u = (up.cross(w)).normalize();
    Vector3 v = w.cross(u);

    Point3 screen_center = pos - (w * screen_distance);
    Point3 top_left_corner =
        screen_center - (u * (screen_width / 2.0)) + (v * (screen_height / 2.0));

    pixel_delta_u_ = u * (screen_width / pixel_width);
    pixel_delta_v_ = v * (screen_height / pixel_height);
    pixel_00_loc_ = top_left_corner + (pixel_delta_u_ * 0.5) - (pixel_delta_v_ * 0.5);
}

} // namespace Prism