/**
 * @file scene.cpp
 * @brief Prism Render Engine
 *
 * @copyright Copyright (c) 2025 src-lua
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "Prism/scene/scene.hpp"

#include "Prism/core/color.hpp"
#include "Prism/core/material.hpp"
#include "Prism/core/style.hpp"
#include "Prism/core/utils.hpp"
#include "Prism/scene/octree.hpp"

#include <chrono>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <thread>

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

Scene::Scene(Camera camera, std::vector<std::unique_ptr<Object>> objects, std::vector<std::unique_ptr<Light>> lights, Color ambient_light, ACCELERATION acceleration)
    : camera_(std::move(camera)), ambient_color_(ambient_light), objects_(std::move(objects)), lights_(std::move(lights)) {
        setAccelerationStructure(acceleration);
    
}

bool get_local_time(std::tm* tm_out, const std::time_t* time_in) {
#if defined(_WIN32) || defined(_MSC_VER)
    // Uses Windows secure version
    return localtime_s(tm_out, time_in) == 0;
#else
    // Uses Linux and macOS (GCC/Clang) secure version
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

double Scene::calculate_shadow_factor(const Light* light, const Point3& p) const {
    if (const auto* area_light = dynamic_cast<const AreaLight*>(light)) {
        // --- Logic for Soft Shadows (AreaLights) ---
        int unblocked_rays = 0;

        for (int i = 0; i < SOFT_SHADOW_SAMPLES; ++i) {
            // Get a new random point on the light's surface for each sample.
            Point3 random_point_on_light = area_light->get_random_point_on_surface();

            double light_distance = (random_point_on_light - p).magnitude();
            Vector3 light_dir = (random_point_on_light - p).normalize();
            Ray shadow_ray(p, light_dir);

            HitRecord rec;
            // If hit_any returns false, the ray was NOT blocked.
            if (!acceleration_structure_->hit_any(shadow_ray, 1e-4, light_distance, rec)) {
                unblocked_rays++;
            }
        }
        return static_cast<double>(unblocked_rays) / SOFT_SHADOW_SAMPLES;

    } else {
        // --- Logic for Hard Shadows (PointLights) ---
        Point3 light_pos = light->get_position();
        double light_distance = (light_pos - p).magnitude();
        Vector3 light_dir = (light_pos - p).normalize();
        Ray shadow_ray(p, light_dir);

        HitRecord rec;
        // If the ray is NOT blocked, it is fully lit (1.0). Otherwise, fully shadowed (0.0).
        if (!acceleration_structure_->hit_any(shadow_ray, 1e-4, light_distance, rec)) {
            return 1.0;
        } else {
            return 0.0;
        }
    }
}


Color Scene::trace(const Ray& ray, int depth) const {
    if (depth <= 0) {
        return Color(0, 0, 0); // Base case for recursion, return black color
    }

    HitRecord rec;
    if (!acceleration_structure_->hit_closest(ray, 1e-4, INFINITY, rec)) {
        return ambient_color_; // Return ambient color if no hit
    }

    auto mat = rec.material;

    Color surface_color = mat->ka * ambient_color_;
    Vector3 view_dir = (ray.origin() - rec.p).normalize();

    for (const auto& light_ptr : lights_) {
        double shadow_factor = calculate_shadow_factor(light_ptr.get(), rec.p);

        if (shadow_factor < 1e-6) {
            continue;
        }

        Vector3 light_dir = (light_ptr->get_position() - rec.p).normalize();

        double diff_factor = std::max(rec.normal.dot(light_dir), 0.0);
        Color diffuse_color = mat->color * diff_factor;

        Color specular_color;
        if (mat->ks.r > 0 || mat->ks.g > 0 || mat->ks.b > 0) { 
            Vector3 reflect_dir = (-light_dir) - rec.normal * 2 * (-light_dir).dot(rec.normal);
            double spec_factor = std::pow(std::max(view_dir.dot(reflect_dir), 0.0), mat->ns);
            specular_color = mat->ks * spec_factor;
        }

        surface_color += (diffuse_color + specular_color) * light_ptr->color * shadow_factor;
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

void Scene::render_tile(std::vector<Color>& buffer, int start_y, int end_y, int& pixels_done, std::mutex& progress_mutex) const {
    const int total_pixels_global = camera_.pixel_height * camera_.pixel_width;
    int last_progress_percent = -1;

    thread_local std::mt19937 generator(std::random_device{}());
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    for (int y = start_y; y < end_y; ++y) {
        for (int x = 0; x < camera_.pixel_width; ++x) {
            Color pixel_color(0, 0, 0);
            for (int s = 0; s < ANTI_ALIASING_SAMPLES; ++s) {
                double random_dx = distribution(generator) - 0.5;
                double random_dy = distribution(generator) - 0.5;

                // Corrigi a chamada de `pixel_00_loc` para ser um acesso de membro.
                Point3 pixel_center = camera_.pixel_00_loc() +
                                      (camera_.pixel_delta_u() * x) -
                                      (camera_.pixel_delta_v() * y);

                Point3 sample_target = pixel_center +
                                       (camera_.pixel_delta_u() * random_dx) +
                                       (camera_.pixel_delta_v() * random_dy);

                Ray sample_ray(camera_.pos, sample_target);
                pixel_color += trace(sample_ray, MAX_DEPTH);
            }

            buffer[y * camera_.pixel_width + x] = pixel_color / static_cast<double>(ANTI_ALIASING_SAMPLES);

            {
                std::lock_guard<std::mutex> lock(progress_mutex); // Trava o mutex
                pixels_done++;
                int current_progress_percent = static_cast<int>((static_cast<double>(pixels_done) / total_pixels_global) * 100.0);
                if (current_progress_percent > last_progress_percent) {
                    last_progress_percent = current_progress_percent;
                    Style::logStatusBar(static_cast<double>(current_progress_percent) / 100.0);
                }
            }
        }
    }
}

void Scene::render() const {
    std::filesystem::path output_dir = "./data/output";
    std::filesystem::create_directories(output_dir);
    auto filename = generate_filename();
    auto full_path = output_dir / filename;
    auto clean_path = std::filesystem::weakly_canonical(output_dir);

    Style::logInfo("Output directory: " + Prism::Style::CYAN + clean_path.string());
    Style::logInfo("Starting render...");

    const int num_threads = std::thread::hardware_concurrency();

    Style::logSection();
    Style::logInfo("--- Render Settings ---");
    Style::logInfo("Threads: " + Prism::Style::CYAN + std::to_string(std::thread::hardware_concurrency()));
    Style::logInfo("Samples per Pixel: " + Prism::Style::CYAN + std::to_string(ANTI_ALIASING_SAMPLES));
    Style::logInfo("Soft Shadow Samples: " + Prism::Style::CYAN + std::to_string(SOFT_SHADOW_SAMPLES));
    Style::logInfo("Max Ray-tracing Depth: " + Prism::Style::CYAN + std::to_string(MAX_DEPTH));
    Style::logInfo("-----------------------");
    Style::logSection();

    auto start_time = std::chrono::steady_clock::now();

    std::vector<std::thread> threads;
    std::vector<Color> image_buffer(camera_.pixel_width * camera_.pixel_height);
    
    int pixels_done = 0;
    std::mutex progress_mutex;

    int rows_per_thread = camera_.pixel_height / num_threads;

    for (int i = 0; i < num_threads; ++i) {
        int start_y = i * rows_per_thread;
        int end_y = (i == num_threads - 1) ? camera_.pixel_height : start_y + rows_per_thread;

        threads.emplace_back(&Scene::render_tile, this, std::ref(image_buffer), start_y, end_y, std::ref(pixels_done), std::ref(progress_mutex));
    }

    for (auto& t : threads) {
        t.join();
    }

    std::ofstream image_file(full_path, std::ios::trunc);
    if (!image_file.is_open()) {
        Style::logError("could not open the file for writing.");
        return;
    }

    image_file << "P3\n" << camera_.pixel_width << " " << camera_.pixel_height << "\n255\n";

    for (const auto& color : image_buffer) {
        image_file << color << "\n";
    }

    image_file.close();

    auto end_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end_time - start_time;

    Style::logDone("Rendering complete.");
    Style::logDone("Total render time: " + Prism::Style::CYAN +
                   std::to_string(elapsed_seconds.count()) + "s");
    Style::logDone("Image saved as: " + Prism::Style::CYAN + full_path.string());
}

void Scene::setAccelerationStructure(ACCELERATION acceleration) {
    
    auto start_time = std::chrono::steady_clock::now();
    
    std::vector<Object*> raw_objects;
    raw_objects.reserve(objects_.size());
    
    for (const auto& obj_ptr : objects_) {
        raw_objects.push_back(obj_ptr.get()); //
    }
    
    switch (acceleration)
    {
        case ACCELERATION::NONE:
        acceleration_structure_ = std::make_unique<NoAcceleration>(raw_objects);
        break;
        case ACCELERATION::OCTREE:
        acceleration_structure_ = std::make_unique<Octree>(raw_objects);
        break;
        default:
        Style::logError("Unsupported acceleration structure type.");
        Style::logError("Falling back to NoAcceleration.");
        acceleration_structure_ = std::make_unique<NoAcceleration>(raw_objects);
        Style::logWarning("This may result in slower rendering performance.");
    }

    Style::logInfo("Acceleration structure set to: " + Prism::Style::CYAN + demangle(typeid(*acceleration_structure_).name()));

    Style::logInfo("Building acceleration structure...");

    auto end_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end_time - start_time;

    Style::logDone("Acceleration structure updated successfully.");
    Style::logDone("Total build time: " + Prism::Style::CYAN + std::to_string(elapsed_seconds.count()) + "s");
}

} // namespace Prism