#include "Prism/scene/scene.hpp"

#include "Prism/core/color.hpp"
#include "Prism/core/material.hpp"
#include "Prism/core/style.hpp"
#include "Prism/core/utils.hpp"

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

Scene::Scene(Camera camera, Color ambient_light)
    : camera_(std::move(camera)), ambient_color_(ambient_light) {
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

bool Scene::is_in_shadow(const std::unique_ptr<Light>& light, const HitRecord& rec) const {
    double light_distance = (light->position - rec.p).magnitude();
    Vector3 light_dir = (light->position - rec.p).normalize();
    Ray shadow_ray(rec.p, light_dir);
    bool in_shadow = false;
    for (const auto& obj_ptr : objects_) {
        HitRecord shadow_rec;
        if (obj_ptr->hit(shadow_ray, 1e-4, light_distance, shadow_rec)) {
            in_shadow = true;
            break;
        }
    }
    return in_shadow;
}

bool Scene::hit_closest(const Ray& ray, double t_min, double t_max, HitRecord& rec) const {
    bool hit_anything = false;
    double closest_t = INFINITY;

    for (const auto& object_ptr : objects_) {
        HitRecord temp_rec;
        if (object_ptr->hit(ray, 1e-4, closest_t, temp_rec)) {
            hit_anything = true;
            closest_t = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

Color Scene::trace(const Ray& ray, int depth) const {
    if (depth <= 0) {
        return Color(0, 0, 0); // Base case for recursion, return black color
    }

    HitRecord rec;
    if (!hit_closest(ray, 1e-4, INFINITY, rec)) {
        return ambient_color_; // Return ambient color if no hit
    }

    auto mat = rec.material;

    Color surface_color = mat->ka * ambient_color_;
    Vector3 view_dir = (ray.origin() - rec.p).normalize();

    for (const auto& light_ptr : lights_) {
        if (!is_in_shadow(light_ptr, rec)) {

            // Diffuse contribution
            Vector3 light_dir = (light_ptr->position - rec.p).normalize();
            double diff_factor = std::max(rec.normal.dot(light_dir), 0.0);
            surface_color += mat->color * diff_factor * light_ptr->color;

            // Specular contribution
            if (mat->ks.r == 0 && mat->ks.g == 0 && mat->ks.b == 0) {
                continue; // Skip specular if ks is black
            }
            Vector3 reflect_dir = (-light_dir) - rec.normal * 2 * (-light_dir).dot(rec.normal);
            double spec_factor = std::pow(std::max(view_dir.dot(reflect_dir), 0.0), mat->ns);
            surface_color += mat->ks * spec_factor * light_ptr->color;
        }
    }

    Color final_color = mat->ke + surface_color;

    // Handle transparency and refraction
    double opacity = mat->d;
    if (opacity < 1.0) {
        double reflectance;
        double refraction_ratio = rec.front_face ? (1.0 / mat->ni) : mat->ni;
        Vector3 unit_direction = ray.direction().normalize();

        double cos_theta = fmin((-unit_direction).dot(rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        if (refraction_ratio * sin_theta > 1.0) {
            reflectance = 1.0;
        } else {
            reflectance = schlick(cos_theta, refraction_ratio);
        }

        Color reflection_color = Color(0, 0, 0);
        Color refraction_color = Color(0, 0, 0);

        Vector3 reflect_dir = ray.direction() - rec.normal * 2 * ray.direction().dot(rec.normal);
        Ray reflection_ray(rec.p + rec.normal * 1e-4, reflect_dir);
        reflection_color = trace(reflection_ray, depth - 1);

        if (reflectance < 1.0) {
            Vector3 refracted_dir = refract(unit_direction, rec.normal, refraction_ratio);
            Ray refracted_ray(rec.p - rec.normal * 1e-4, refracted_dir);
            refraction_color = trace(refracted_ray, depth - 1);
        }

        Color trasmited_color =
            reflection_color * reflectance + refraction_color * (1.0 - reflectance);
        final_color = final_color * opacity + trasmited_color * (1.0 - opacity);
    } else if (mat->ks.r > 0 || mat->ks.g > 0 || mat->ks.b > 0) {
        Vector3 reflect_dir = ray.direction() - rec.normal * 2 * ray.direction().dot(rec.normal);
        Ray reflection_ray(rec.p + rec.normal * 1e-4, reflect_dir);
        final_color = final_color * (1.0 - mat->ks.r) + mat->ks * trace(reflection_ray, depth - 1);
    }

    return final_color.clamp();
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

    auto start_time = std::chrono::steady_clock::now();

    image_file << "P3\n" << camera_.pixel_width << " " << camera_.pixel_height << "\n255\n";

    int total_pixels = camera_.pixel_height * camera_.pixel_width;
    int pixels_done = 0;
    const int progress_bar_width = 25;
    int last_progress_percent = -1;

    for (Ray const& ray : camera_) {
        image_file << trace(ray, 5) << '\n';

        pixels_done++;
        int current_progress_percent =
            static_cast<int>((static_cast<double>(pixels_done) / total_pixels) * 100.0);

        if (current_progress_percent > last_progress_percent) {
            last_progress_percent = current_progress_percent;
            Style::logStatusBar(static_cast<double>(current_progress_percent) / 100.0);
        }
    }
    image_file.close();

    auto end_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end_time - start_time;

    Style::logDone("Rendering complete.");
    Style::logDone("Total render time: " + Prism::Style::CYAN + std::to_string(elapsed_seconds.count()) + "s");
    Style::logDone("Image saved as: " + Prism::Style::CYAN + full_path.string());

}

} // namespace Prism