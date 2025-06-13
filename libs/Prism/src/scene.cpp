#include "Prism/scene.hpp"
#include "Prism/color.hpp"
#include "Prism/material.hpp"
#include <fstream>
#include <iostream>
#include <cmath>
#include <chrono>
#include <iomanip>
#include <filesystem>    

namespace Prism {

Scene::Scene(Camera camera) : camera_(std::move(camera)) {}

void Scene::addObject(std::unique_ptr<Object> object) {
    objects_.push_back(std::move(object));
}

std::filesystem::path generate_filename() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << "render_" << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d_%H-%M-%S") << ".ppm";
    std::string filename = ss.str();
    
    return filename;
}

void Scene::render() const {
    std::filesystem::path output_dir = "./data/output";
    std::filesystem::create_directories(output_dir);
    auto filename = generate_filename();
    auto full_path = output_dir / filename;
    
    std::ofstream image_file(full_path, std::ios::trunc);
    if (!image_file.is_open()) {
        std::cerr << "Error: Could not open the file for writing.\n";
        return;
    }

    image_file << "P3\n"
               << camera_.pixel_width << " " << camera_.pixel_height << "\n255\n";

    std::clog << "Starting render...\n";

    int pixels_done = 0;

    for (Ray const &ray : camera_) {
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
            pixel_color = closest_hit_rec.material->color;
        } else {
            Vector3 unit_direction = ray.direction().normalize();
            pixel_color = Color(0,0,0);
        }
        
        image_file << pixel_color << '\n';

        if (++pixels_done % camera_.pixel_width == 0) {
            double percent = (static_cast<double>(pixels_done) / (camera_.pixel_height * camera_.pixel_width)) * 100.0;
            std::clog << "\rProgress: " << static_cast<int>(percent) << "%" << std::flush;
        }
    }

    std::clog << "\nRendering complete.\nImage saved as " << filename << ".\n";
    image_file.close();
}

} // namespace Prism