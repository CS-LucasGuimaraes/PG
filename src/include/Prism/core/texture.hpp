#ifndef PRISM_TEXTURE_HPP_
#define PRISM_TEXTURE_HPP_

#include "prism_export.h"
#include "Prism/core/color.hpp"
#include "Prism/core/point.hpp"
#include <memory>
#include <cmath>

namespace Prism {

class PRISM_EXPORT Texture {
public:
    virtual ~Texture() = default;
    virtual Color value(double u, double v, const Point3& p) const = 0;
};

class PRISM_EXPORT SolidColor : public Texture {
public:
    SolidColor(Color c) : color_value(c) {}
    SolidColor(double red, double green, double blue) : SolidColor(Color(red, green, blue)) {}

    virtual Color value(double u, double v, const Point3& p) const override {
        return color_value;
    }

private:
    Color color_value;
};

class PRISM_EXPORT CheckerTexture : public Texture {
public:
    CheckerTexture(double scale, std::shared_ptr<Texture> even, std::shared_ptr<Texture> odd)
        : inv_scale(1.0 / scale), even(even), odd(odd) {}

    CheckerTexture(double scale, Color c1, Color c2)
        : inv_scale(1.0 / scale),
          even(std::make_shared<SolidColor>(c1)),
          odd(std::make_shared<SolidColor>(c2)) {}

    virtual Color value(double u, double v, const Point3& p) const override {
        auto uInteger = static_cast<int>(std::floor(inv_scale * u));
        auto vInteger = static_cast<int>(std::floor(inv_scale * v));

        bool isEven = (uInteger + vInteger) % 2 == 0;

        return isEven ? even->value(u, v, p) : odd->value(u, v, p);
    }

private:
    double inv_scale;
    std::shared_ptr<Texture> even;
    std::shared_ptr<Texture> odd;
};

} // namespace Prism

#endif // PRISM_TEXTURE_HPP_