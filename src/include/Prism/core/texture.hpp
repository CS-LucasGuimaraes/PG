#ifndef PRISM_TEXTURE_HPP_
#define PRISM_TEXTURE_HPP_

#include "Prism/core/color.hpp"
#include "Prism/core/point.hpp"
#include "Prism/core/style.hpp"
#include "prism_export.h"

#include <stb/stb_image.h>

#include <algorithm>
#include <cmath>
#include <memory>
#include <string>
#include <vector>

namespace Prism {

class PRISM_EXPORT Texture {
  public:
    virtual ~Texture() = default;
    virtual Color value(double u, double v, const Point3& p) const = 0;
};

class PRISM_EXPORT SolidColor : public Texture {
  public:
    SolidColor(Color c) : color_value(c) {
    }
    SolidColor(double red, double green, double blue) : SolidColor(Color(red, green, blue)) {
    }

    virtual Color value(double u, double v, const Point3& p) const override {
        return color_value;
    }

  private:
    Color color_value;
};

class PRISM_EXPORT CheckerTexture : public Texture {
  public:
    CheckerTexture(double scale, std::shared_ptr<Texture> even, std::shared_ptr<Texture> odd)
        : inv_scale(1.0 / scale), even(even), odd(odd) {
    }

    CheckerTexture(double scale, Color c1, Color c2)
        : inv_scale(1.0 / scale), even(std::make_shared<SolidColor>(c1)),
          odd(std::make_shared<SolidColor>(c2)) {
    }

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

class PRISM_EXPORT ImageTexture : public Texture {
  public:
    ImageTexture(const std::string& filename, double scale = 1.0) : inv_scale(1.0 / scale) {
        Prism::Style::logInfo("ImageTexture CONSTRUCTOR: Received scale = " +
                              std::to_string(scale) + ", inv_scale = " + std::to_string(inv_scale));
        int channels_in_file;
        // Carrega a imagem e força para ter 3 canais (RGB)
        unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels_in_file, 3);

        if (!data) {
            // Usa o log de erros do seu motor
            Prism::Style::logError("Could not load texture file '" + filename + "'.");
            width = height = 0;
        }

        image_data.assign(data, data + width * height * 3);
        stbi_image_free(data);
    }

    virtual Color value(double u, double v, const Point3& p) const override {
        // Se a imagem não carregou, retorna uma cor de erro (magenta)
        if (image_data.empty()) {
            Prism::Style::logError("Image texture not loaded properly. Returning magenta color.");
            return Color(1, 0, 1);
        }

        u = u * inv_scale;
        v = v * inv_scale;

        u = u - floor(u);
        v = 1.0 - (v - floor(v));

        int i = static_cast<int>(u * width);
        int j = static_cast<int>(v * height);

        i = std::clamp(i, 0, width - 1);
        j = std::clamp(j, 0, height - 1);

        const double color_scale = 1.0 / 255.0;
        int pixel_offset = j * width * 3 + i * 3;

        return Color(color_scale * image_data[pixel_offset + 0],
                     color_scale * image_data[pixel_offset + 1],
                     color_scale * image_data[pixel_offset + 2]);
    }

  private:
    int width = 0;
    int height = 0;
    double inv_scale;
    std::vector<unsigned char> image_data;
};

} // namespace Prism

#endif // PRISM_TEXTURE_HPP_