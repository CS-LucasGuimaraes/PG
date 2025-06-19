#ifndef PRISM_CAMERA_HPP_
#define PRISM_CAMERA_HPP_

#include "Prism/point.hpp"
#include "Prism/ray.hpp"
#include "Prism/vector.hpp"
#include "Prism/matrix.hpp"
#include "prism_export.h"
#include <initializer_list>
#include <iterator>
namespace Prism {

/**
 * @class Camera
 * @brief Represents a camera in 3D space
 *
 * The Point3 class represents a point in 3D space with x, y, and z coordinates.
 */
class PRISM_EXPORT Camera {
  public:
    /**
     * @brief Constructs a Camera with specified parameters.
     * @param position The starting position of the camera in 3D space.
     * @param target The point at which the camera is aiming (the look-at target).
     * @param upvec The up direction vector that starts from the camera and points upwards.
     * @param distance The distance between the camera and the projection screen (view plane).
     * @param viewport_height The height of the viewport (projection screen).
     * @param viewport_width The width of the viewport (projection screen).
     * @param image_height The height of the image in pixels.
     * @param image_width The width of the image in pixels.
     */
    Camera(const Point3& position, const Point3& target, const Vector3& upvec,
           double distance, double viewport_height, double viewport_width,
           int image_height, int image_width);

    class CameraIterator {
      public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Ray;
        using difference_type = std::ptrdiff_t;
        using pointer = Ray*;
        using reference = Ray&;

        CameraIterator(const Camera* cam, int y, int x) : camera(cam), current_y(y), current_x(x) {
        }

        Ray operator*() const {
            Point3 pixel_center = camera->pixel_00_loc + (camera->pixel_delta_u * current_x) -
                                  (camera->pixel_delta_v * current_y);

            return Ray(camera->pos, pixel_center);
        }

        CameraIterator& operator++() {
            current_x++;
            if (current_x >= camera->pixel_width) {
                current_x = 0;
                current_y++;
            }
            return *this;
        }

        bool operator!=(const CameraIterator& other) const {
            return current_y != other.current_y || current_x != other.current_x;
        }

      private:
        const Camera* camera;
        int current_y;
        int current_x;
    };

    CameraIterator begin() {
        return CameraIterator(this, 0, 0);
    }
    CameraIterator end() {
        return CameraIterator(this, pixel_height, 0);
    }

    CameraIterator begin() const {
        return CameraIterator(this, 0, 0);
    }
    CameraIterator end() const {
        return CameraIterator(this, pixel_height, 0);
    }

    Point3 pos;
    Point3 aim;
    Vector3 up;

    Matrix coordinate_basis;

    double screen_distance;
    double screen_height;
    double screen_width;

    int pixel_height;
    int pixel_width;

  private:
    Point3 pixel_00_loc;
    Vector3 pixel_delta_u;
    Vector3 pixel_delta_v;
};

} // namespace Prism

#endif // PRISM_CAMERA_HPP_