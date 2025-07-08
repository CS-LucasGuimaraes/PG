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

    /**
     * @brief Constructs a Camera with default parameters.
     * This constructor initializes the camera at the origin, looking down the negative z-axis,
     * with the up vector pointing in the positive y direction.
     */
    Camera() : Camera(Point3(0, 0, 0), Point3(0, 0, -1), Vector3(0, 1, 0), 1.0, 1.0, 1.0, 480, 640) {}
    
    /**
     * @class CameraIterator
     * @brief An iterator for traversing the pixels of the camera's view.
     *
     * This iterator allows iteration over the rays that correspond to each pixel in the camera's view.
     * It provides a way to access each pixel's ray in a forward manner.
     */
    class CameraIterator {
      public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Ray;
        using difference_type = std::ptrdiff_t;
        using pointer = Ray*;
        using reference = Ray&;

        /**
         * @brief Constructs a CameraIterator for the given camera, starting at pixel (y, x).
         * @param cam Pointer to the Camera object.
         * @param y The starting row (y-coordinate) of the pixel.
         * @param x The starting column (x-coordinate) of the pixel.
         */
        CameraIterator(const Camera* cam, int y, int x) : camera(cam), current_y(y), current_x(x) {
        }

        /**
         * @brief Dereferences the iterator to get the Ray corresponding to the current pixel.
         * @return A Ray object representing the ray for the current pixel.
         */
        Ray operator*() const {
            Point3 pixel_center = camera->pixel_00_loc + (camera->pixel_delta_u * current_x) -
                                  (camera->pixel_delta_v * current_y);

            return Ray(camera->pos, pixel_center);
        }

        /**
         * @brief Post-increment operator to move the iterator to the next pixel.
         * @return A reference to the updated CameraIterator.
         */
        CameraIterator& operator++() {
            current_x++;
            if (current_x >= camera->pixel_width) {
                current_x = 0;
                current_y++;
            }
            return *this;
        }

        /**
         * @brief Difference operator to compare two CameraIterator objects.
         * @param other The other CameraIterator to compare with.
         * @return True if the current iterator is not equal to the other, false otherwise.
         */
        bool operator!=(const CameraIterator& other) const {
            return current_y != other.current_y || current_x != other.current_x;
        }

      private:
        const Camera* camera; ///< Pointer to the Camera object being iterated over
        int current_y; ///< Current row (y-coordinate) of the pixel
        int current_x; ///< Current column (x-coordinate) of the pixel
    };

    /**
     * @brief Returns a const iterator to the beginning of the camera's pixel rays.
     * @return A CameraIterator pointing to the first pixel ray.
     *
     * This function allows iteration over the rays corresponding to each pixel
     * in the camera's view. It works for both const and non-const Camera objects.
     */
    CameraIterator begin() const {
        return CameraIterator(this, 0, 0);
    }

    /**
     * @brief Returns a const iterator to the end of the camera's pixel rays.
     * @return A CameraIterator pointing to one past the last pixel ray.
     */
    CameraIterator end() const {
        return CameraIterator(this, pixel_height, 0);
    }

    Point3 pos; ///< Camera position in 3D space
    Point3 aim; ///< Point the camera is looking at (the target point)
    Vector3 up; ///< Up vector indicating the upward direction from the camera

    Matrix coordinate_basis; ///< Coordinate basis matrix for the camera, defining its orientation in space

    double screen_distance; ///< Distance from the camera to the projection screen (view plane)
    double screen_height; ///< Height of the projection screen (view plane)
    double screen_width; ///< Width of the projection screen (view plane)

    int pixel_height; ///< Height of the image in pixels
    int pixel_width; ///< Width of the image in pixels

  private:
    Point3 pixel_00_loc; ///< Location of the pixel at (0, 0) in the camera's view
    Vector3 pixel_delta_u; ///< Vector representing the change in position along the u direction (horizontal)
    Vector3 pixel_delta_v; ///< Vector representing the change in position along the v direction (vertical)
};

} // namespace Prism

#endif // PRISM_CAMERA_HPP_