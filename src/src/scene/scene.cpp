#include "Prism/scene/scene.hpp"

#include "Prism/core/color.hpp"
#include "Prism/core/material.hpp"
#include "Prism/core/style.hpp"

#include <chrono>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace Prism {

PRISM_EXPORT int convert_color(double f) {
    return static_cast<int>(255.999 * f);
}

PRISM_EXPORT std::ostream& operator<<(std::ostream& os, const Color& color) {
    os << static_cast<int>(convert_color(color.r)) << " "
       << static_cast<int>(convert_color(color.g)) << " "
       << static_cast<int>(convert_color(color.b));
    return os;
}

Scene::Scene(Camera camera, Color ambient_light) : camera_(std::move(camera)), ambient_color_(ambient_light) {
}

void Scene::addObject(std::unique_ptr<Object> object) {
    objects_.push_back(std::move(object));
}

void Scene::addLight(std::unique_ptr<Light> light) {
    lights_.push_back(std::move(light));
}

bool get_local_time(std::tm* tm_out, const std::time_t* time_in) {
#if defined(_WIN32) || defined(_MSC_VER)
    // Usa a versão segura do Windows (MSVC)
    return localtime_s(tm_out, time_in) == 0;
#else
    // Usa a versão reentrante/segura do Linux e macOS (GCC/Clang)
    return localtime_r(time_in, tm_out) != nullptr;
#endif
}

std::filesystem::path generate_filename() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::tm time_info;

    if (get_local_time(&time_info, &in_time_t)) {
        std::stringstream ss;
        ss << "render_" << std::put_time(&time_info, "%Y-%m-%d_%H-%M-%S") << ".ppm";
        return ss.str();
    }

    Style::logError("Could not get local time for filename generation.");
    Style::logWarning("Using fallback filename.");
    return "render_fallback.ppm";
}

void Scene::render() const {
    std::filesystem::path output_dir = "./data/output";
    std::filesystem::create_directories(output_dir);
    auto filename = generate_filename();
    auto full_path = output_dir / filename;

    std::ofstream image_file(full_path, std::ios::trunc);
    if (!image_file.is_open()) {
        Style::logError("could not open the file for writing.");
        return;
    }

    auto clean_path = std::filesystem::weakly_canonical(output_dir);

    Style::logInfo("Output directory: " + Prism::Style::CYAN + clean_path.string());
    Style::logInfo("Starting render...\n");

    image_file << "P3\n" << camera_.pixel_width << " " << camera_.pixel_height << "\n255\n";

    int total_pixels = camera_.pixel_height * camera_.pixel_width;
    int pixels_done = 0;
    const int progress_bar_width = 25;
    int last_progress_percent = -1;

    for (Ray const& ray : camera_) {
        HitRecord closest_hit_rec;
        bool hit_anything = false;
        double closest_t = INFINITY;

        for (const auto& object_ptr : objects_) {
            HitRecord temp_rec;
            if (object_ptr->hit(ray, 0.001, closest_t, temp_rec)) {
                hit_anything = true;
                closest_t = temp_rec.t;
                closest_hit_rec = temp_rec;
            }
        }

        Color pixel_color;
        if (hit_anything) {
            Point3 hit_point = closest_hit_rec.p;
            Vector3 normal = closest_hit_rec.normal;
            Vector3 view_dir = (ray.origin() - hit_point).normalize();
            auto mat = closest_hit_rec.material;

            for (const auto& light_ptr : lights_) {
                Vector3 light_dir = (light_ptr->position - hit_point).normalize();
                double light_distance = (light_ptr->position - hit_point).magnitude();

                Ray shadow_ray(hit_point, light_dir);
                bool in_shadow = false;
                for (const auto& obj_ptr : objects_) {
                    HitRecord shadow_rec;
                    if (obj_ptr->hit(shadow_ray, 0.001, light_distance, shadow_rec)) {
                        in_shadow = true;
                        break;
                    }
                }

                Color ambient = mat->ka * this->ambient_color_;

                if (in_shadow) {
                    pixel_color += ambient;
                    continue;
                }

                double diff_factor = std::max(0.0, normal.dot(light_dir));
                Color diffuse = mat->color * diff_factor * light_ptr->color;

                Vector3 reflect_dir = (normal * 2 * (light_dir.dot(normal))) - light_dir;
                double spec_factor = std::pow(std::max(0.0, view_dir.dot(reflect_dir)), mat->ns);
                Color specular = mat->ks * spec_factor * light_ptr->color;

                pixel_color += ambient + diffuse + specular;
            }

            pixel_color.clamp();

        } else {
            pixel_color = Color(0, 0, 0);
        }

        image_file << pixel_color << '\n';

        pixels_done++;
        int current_progress_percent =
            static_cast<int>((static_cast<double>(pixels_done) / total_pixels) * 100.0);

        if (current_progress_percent > last_progress_percent) {
            last_progress_percent = current_progress_percent;
            Style::logStatusBar(static_cast<double>(current_progress_percent) / 100.0);
        }
    }

    image_file.close();

    Style::logDone("Rendering complete.");
    Style::logDone("Image saved as: " + Prism::Style::CYAN + full_path.string());
}

} // namespace Prism