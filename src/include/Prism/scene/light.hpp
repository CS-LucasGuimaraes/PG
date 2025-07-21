/**
 * @file light.hpp
 * @brief Prism Render Engine
 *
 * @copyright Copyright (c) 2025 src-lua
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef PRISM_LIGHT_HPP_
#define PRISM_LIGHT_HPP_

#include "prism_export.h"

#include "Prism/core/color.hpp"
#include "Prism/core/point.hpp"
#include "Prism/core/vector.hpp"

#include <cmath>
#include <random>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace Prism {

/**
 * @class Light
 * @brief Base class for all light sources in the scene.
 * This class defines the basic properties and interface for light sources in the scene.
 * It is intended to be inherited by specific types of lights, such as PointLight and AreaLight.
 */
class PRISM_EXPORT Light {
  public:
    Color color; ///< Color and intensity of the light source.

    Light(const Color& col) : color(col) {}
    virtual ~Light() = default;

    /**
     * @brief Returns the "representative" position of the light (usually the center).
     */
    virtual Point3 get_position() const = 0;
};


/**
 * @class PointLight
 * @brief Represents a point light source in the scene.
 */
class PRISM_EXPORT PointLight : public Light {
  public:
    Point3 position; ///< Position of the point light in 3D space.

    PointLight(const Point3& pos, const Color& col) : Light(col), position(pos) {}

    /**
     * @brief Returns the position of the point light.
     * This is the position in 3D space where the light is located.
     * @return The position of the point light.
     */
    virtual Point3 get_position() const override {
        return position;
    }
};


/**
 * @class AreaLight
 * @brief Base class for area lights in the scene.
 * Area lights are defined by a surface area and can provide soft shadows.
 * This class is intended to be inherited by specific area light types, such as QuadLight and SphericalLight.
 */
class PRISM_EXPORT AreaLight : public Light {
  public:
    AreaLight(const Color& col) : Light(col) {}

    /**
     * @brief Returns a random point on the surface of the area light.
     * This method is used to sample points on the light's surface for shadow calculations and light sampling.
     * @return A random point on the surface of the area light.
     * The implementation of this method will depend on the specific type of area light.
     */
    virtual Point3 get_random_point_on_surface() const = 0;
};


/**
 * @class QuadLight
 * @brief Represents a rectangular area light source.
 */
class PRISM_EXPORT QuadLight : public AreaLight {
  public:
    Point3 corner;  ///< The corner point of the rectangle defining the light's area.
    Vector3 u_vec;  ///< Vector that defines the width of the rectangle.
    Vector3 v_vec;  ///< Vector that defines the height and orientation of the other side.

    QuadLight(const Point3& corn, const Vector3& u, const Vector3& v, const Color& col)
        : AreaLight(col), corner(corn), u_vec(u), v_vec(v) {}

    /**
     * @brief Returns the position of the quad light.
     * This is the center point of the rectangle defined by the corner and the vectors.
     * @return The position of the quad light.
     */
    virtual Point3 get_position() const override {
        return corner + (u_vec * 0.5) + (v_vec * 0.5);
    }

    /**
     * @brief Returns a random point on the surface of the rectangle.
     * This method generates a random point uniformly distributed over the rectangle's area.
     * @return A random point on the surface of the quad light.
     */
    virtual Point3 get_random_point_on_surface() const override {
        thread_local std::mt19937 generator(std::random_device{}());
        std::uniform_real_distribution<double> distribution(0.0, 1.0);
        return corner + (u_vec * distribution(generator)) + (v_vec * distribution(generator));
    }
};


/**
 * @class SphericalLight
 * @brief A spherical area light source.
 */
class PRISM_EXPORT SphericalLight : public AreaLight {
  public:
    Point3 center; ///< The center of the sphere defining the light's area.
    double radius; ///< The radius of the sphere defining the light's area.

    SphericalLight(const Point3& c, double r, const Color& col)
        : AreaLight(col), center(c), radius(r) {}


    /**
     * @brief Returns the position of the spherical light.
     * This is the center of the sphere defining the light's area.
     * @return The position of the spherical light.
     */
    virtual Point3 get_position() const override {
        return center;
    }

    /**
     * @brief Returns a random point on the surface of the sphere.
     * This method generates a random point uniformly distributed over the sphere's surface.
     * @return A random point on the surface of the spherical light.
     */
    virtual Point3 get_random_point_on_surface() const override {
        thread_local std::mt19937 generator(std::random_device{}());
        std::uniform_real_distribution<double> dist(0.0, 1.0);

        double theta = 2.0 * M_PI * dist(generator);
        double u = dist(generator);

        double z_local = 1.0 - 2.0 * u;
        double radius_proj = std::sqrt(1.0 - z_local * z_local);

        // Converte para coordenadas cartesianas
        double x = radius * radius_proj * std::cos(theta);
        double y = radius * radius_proj * std::sin(theta);
        double z = radius * z_local;

        return center + Vector3(x, y, z);
    }
};

} // namespace Prism

#endif // PRISM_LIGHT_HPP_