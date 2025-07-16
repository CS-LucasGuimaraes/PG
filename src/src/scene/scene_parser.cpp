#ifdef PRISM_BUILD_SCENE

#include "Prism/scene/scene_parser.hpp"

#include "Prism/core/color.hpp"
#include "Prism/core/material.hpp"
#include "Prism/core/matrix.hpp"
#include "Prism/core/point.hpp"
#include "Prism/core/style.hpp"
#include "Prism/core/vector.hpp"
#include "Prism/objects/mesh.hpp"
#include "Prism/objects/plane.hpp"
#include "Prism/objects/sphere.hpp"
#include "Prism/objects/triangle.hpp"
#include "Prism/scene/camera.hpp"

#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
#include <yaml-cpp/yaml.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846 // Define M_PI if not already defined
#endif

namespace Prism {

// --- Parsing Helper Functions ---

// Converts a YAML node [x, y, z] to a Point3
Point3 parsePoint(const YAML::Node& node) {
    if (!node.IsSequence() || node.size() != 3) {
        throw std::runtime_error("Parsing error: Malformed 3D point.");
    }
    return Point3(node[0].as<double>(), node[1].as<double>(), node[2].as<double>());
}

// Converts a YAML node [x, y, z] to a Vector3
Vector3 parseVector(const YAML::Node& node) {
    if (!node.IsSequence() || node.size() != 3) {
        throw std::runtime_error("Parsing error: Malformed 3D vector.");
    }
    return Vector3(node[0].as<double>(), node[1].as<double>(), node[2].as<double>());
}

Color parseColor(const YAML::Node& node) {
    if (!node.IsSequence() || node.size() != 3) {
        throw std::runtime_error("Parsing error: Malformed color.");
    }
    return Color(node[0].as<double>(), node[1].as<double>(), node[2].as<double>());
}

// Converts a YAML node with material properties to a Material
std::shared_ptr<Material> parseMaterial(const YAML::Node& node) {
    auto mat = std::make_shared<Material>();

    if (node["color"]) {
        Vector3 v = parseVector(node["color"]);
        mat->color = Color(v.x, v.y, v.z);
    }
    if (node["ka"])
        mat->ka = parseColor(node["ka"]);
    if (node["ks"])
        mat->ks = parseColor(node["ks"]);
    if (node["ke"])
        mat->ke = parseColor(node["ke"]);
    if (node["ns"])
        mat->ns = node["ns"].as<double>();
    if (node["ni"])
        mat->ni = node["ni"].as<double>();
    if (node["d"])
        mat->d = node["d"].as<double>();
    return mat;
}

// Converts a YAML list of transformations into a single transformation matrix
Matrix parseTransformations(const YAML::Node& node) {
    Matrix final_transform = Matrix::identity(4);
    if (!node)
        return final_transform;

    // Transformations are applied in the reverse order of the list (standard in computer graphics)
    for (int i = node.size() - 1; i >= 0; --i) {
        const auto& transform_node = node[i];
        std::string type = transform_node["type"].as<std::string>();
        Matrix current_transform = Matrix::identity(4);

        if (type == "translation") {
            Vector3 v = parseVector(transform_node["vector"]);
            current_transform = Matrix::translation(v.x, v.y, v.z);
        } else if (type == "rotation") {
            double angle_deg = transform_node["angle"].as<double>();
            double angle_rad = angle_deg * (M_PI / 180.0); // Convert to radians
            current_transform = Matrix::rotation(angle_rad, parseVector(transform_node["axis"]));
        } else if (type == "scaling") {
            Vector3 v = parseVector(transform_node["factors"]);
            current_transform = Matrix::scaling(v.x, v.y, v.z);
        } else {
            Style::logWarning("Unknown transformation type: " + type +
                              ". Skipping this transformation.");
            continue; // Skip unknown transformation types
        }
        final_transform = final_transform * current_transform;
    }
    return final_transform;
}

// --- SceneParser Class Implementation ---

SceneParser::SceneParser(const std::string& sceneFilePath) : filePath(sceneFilePath) {
}

Scene SceneParser::parse() {
    YAML::Node root;
    try {
        root = YAML::LoadFile(filePath);
    } catch (const YAML::Exception& e) {
        throw std::runtime_error("Error loading YAML file: " + std::string(e.what()));
    }

    Style::logInfo("Parsing scene from file: " + Style::CYAN + filePath);

    // Parse the Camera
    if (!root["camera"]) {
        throw std::runtime_error("'camera' node not found in the scene file.");
    }
    const auto& cam_node = root["camera"];
    Camera camera(parsePoint(cam_node["lookfrom"]), parsePoint(cam_node["lookat"]),
                  parseVector(cam_node["vup"]), cam_node["screen_distance"].as<double>(),
                  cam_node["viewport_height"].as<double>(), cam_node["viewport_width"].as<double>(),
                  cam_node["image_height"].as<int>(), cam_node["image_width"].as<int>());

    Color ambient_light(0.1, 0.1, 0.1); // Valor padrão
    if (root["ambient_light"]) {
        Vector3 v = parseVector(root["ambient_light"]);
        ambient_light = Color(v.x, v.y, v.z);
    } else {
        Style::logWarning("Ambient light not defined. Using default (0.1, 0.1, 0.1).");
    }

    Scene scene(std::move(camera), ambient_light);

    // Parse Material Definitions (for reuse)
    std::map<std::string, std::shared_ptr<Material>> materials;
    if (root["definitions"] && root["definitions"]["materials"]) {
        for (const auto& mat_node : root["definitions"]["materials"]) {
            std::string name = mat_node.first.as<std::string>();
            materials[name] = parseMaterial(mat_node.second);
        }
    }

    if (root["lights"] && root["lights"].IsSequence()) {
        for (const auto& light_node : root["lights"]) {
            Point3 pos = parsePoint(light_node["position"]);
            Vector3 color_vec = parseVector(light_node["color"]);
            Color color(color_vec.x, color_vec.y, color_vec.z);
            scene.addLight(std::make_unique<Light>(pos, color));
        }
    }
    else {
        Style::logWarning("'lights' node not found or is not a list. No lights will be added.");
    }

    // Parse Objects
    if (!root["objects"] || !root["objects"].IsSequence()) {
        throw std::runtime_error("'objects' node not found or is not a list.");
    }

    for (const auto& obj_node : root["objects"]) {
        std::string type = obj_node["type"].as<std::string>();

        // Find the material (whether defined inline or by reference)
        std::shared_ptr<Material> material;
        if (obj_node["material"].IsMap()) {
            material = parseMaterial(obj_node["material"]);
        } else if (obj_node["material"].IsScalar()) {
            std::string mat_name = obj_node["material"].as<std::string>();
            if (materials.count(mat_name)) {
                material = materials.at(mat_name);
            } else {
                throw std::runtime_error("Referenced material not found: " + mat_name);
            }
        } else {
            material = std::make_shared<Material>(); // Default material
        }

        std::unique_ptr<Object> object;

        if (type == "sphere") {
            object = std::make_unique<Sphere>(parsePoint(obj_node["center"]),
                                              obj_node["radius"].as<double>(), material);
        } else if (type == "plane") {
            object = std::make_unique<Plane>(parsePoint(obj_node["point_on_plane"]),
                                             parseVector(obj_node["normal"]), material);
        } else if (type == "triangle") {
            object =
                std::make_unique<Triangle>(parsePoint(obj_node["p1"]), parsePoint(obj_node["p2"]),
                                           parsePoint(obj_node["p3"]), material);
        } else if (type == "mesh") {
            std::filesystem::path scene_dir = std::filesystem::path(filePath).parent_path();
            std::string mesh_path_str = obj_node["path"].as<std::string>();
            std::filesystem::path full_mesh_path = scene_dir / mesh_path_str;

            object = std::make_unique<Mesh>(full_mesh_path);
            // Overrides the .obj material with the one from the .yml, if specified
            if (obj_node["material"]) {
                auto mesh_ptr = static_cast<Mesh*>(object.get());
                mesh_ptr->setMaterial(material);
            }
        } else {
            Style::logWarning("Unknown object type: " + type + ". Skipping this object.");
            continue;
        }

        if (object) {
            object->setTransform(parseTransformations(obj_node["transform"]));
            scene.addObject(std::move(object));
        }
    }

    return scene;
}

} // namespace Prism

#endif // PRISM_BUILD_SCENE