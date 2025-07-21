/**
 * @file ObjReader.hpp
 * @brief Prism Render Engine
 *
 * @copyright Copyright (c) 2025 src-lua
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef PRISM_OBJREADER_HPP_
#define PRISM_OBJREADER_HPP_

#include "prism_export.h"

#include "Prism/objects/Colormap.hpp"
#include "Prism/core/texture.hpp"

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace Prism {

class ObjReader {
  public:
    struct FaceIndices {
        std::array<unsigned int, 3> vertex_indices;
        std::array<unsigned int, 3> normal_indices;
    };

    std::shared_ptr<Material> curMaterial;
    std::vector<std::array<double, 3>> vertices;
    std::vector<std::array<double, 3>> normals;
    std::vector<FaceIndices> faces;

    ObjReader(const std::string& filename) {
        curMaterial = std::make_shared<Material>();

        file.open(filename);
        if (!file.is_open()) {
            Style::logError("Erro ao abrir o arquivo: " + filename);
            return;
        }

        std::string line, mtlfile, colorname, filename_mtl;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string prefix;
            iss >> prefix;

            if (prefix == "mtllib") {
                iss >> filename_mtl;
                std::string filename_mtl_path = filename;
                filename_mtl_path.replace(filename.length() - 3, 3, "mtl");
                cmap = colormap(filename_mtl_path);
            } else if (prefix == "usemtl") {
                iss >> colorname;
                auto mtlProps = cmap.getMaterial(colorname);
                curMaterial = std::make_shared<Material>(
                    mtlProps.texture->value(0.0, 0.0, Point3()),
                    mtlProps.ka,
                    mtlProps.ks, mtlProps.ke, mtlProps.ns, mtlProps.ni, mtlProps.d);
            } else if (prefix == "v") {
                double x, y, z;
                iss >> x >> y >> z;
                vertices.push_back({x, y, z});
            } else if (prefix == "vn") {
                double nx, ny, nz;
                iss >> nx >> ny >> nz;
                normals.push_back({nx, ny, nz});
            } else if (prefix == "f") {
                FaceIndices face;
                std::string vertex_info;
                for (int i = 0; i < 3; ++i) {
                    iss >> vertex_info;
                    std::replace(vertex_info.begin(), vertex_info.end(), '/', ' ');
                    std::istringstream v_iss(vertex_info);
                    unsigned int t_idx;
                    v_iss >> face.vertex_indices[i] >> t_idx >> face.normal_indices[i];
                    --face.vertex_indices[i]; // Convert to 0-based index
                    --face.normal_indices[i]; // Convert to 0-based index
                }
                faces.push_back(face);
            }
        }

        file.close();
    }

  private:
    std::ifstream file;
    colormap cmap;

    // /**
    //  * @brief Prints each triangle’s vertices to the console
    //  */
    // void print_faces() const {
    //     int i = 0;
    //     for (const auto& tri : triangles) {
    //         std::clog << "Face " << (++i) << ": ";
    //         std::cout << "(" << tri.getPoint1().x << ", " << tri.getPoint1().y << ", " <<
    //         tri.getPoint1().z << ") "; std::cout << "(" << tri.getPoint2().x << ", " <<
    //         tri.getPoint2().y << ", " << tri.getPoint2().z << ") "; std::cout << "(" <<
    //         tri.getPoint3().x << ", " << tri.getPoint3().y << ", " << tri.getPoint3().z << ") ";
    //         std::cout << std::flush;
    //         std::clog << std::endl;
    //     }
    // }
};

} // namespace Prism

#endif // PRISM_OBJREADER_HPP_
