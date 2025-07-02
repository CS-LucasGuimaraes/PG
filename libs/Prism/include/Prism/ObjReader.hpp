#ifndef PRISM_OBJREADER_HPP_
#define PRISM_OBJREADER_HPP_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <memory>
#include "Prism/vector.hpp"
#include "Prism/point.hpp"
#include "Prism/Colormap.hpp"
#include "Prism/triangle.hpp"
#include "Prism/material.hpp"
#include "prism_export.h"

namespace Prism {

class ObjReader {
  public:
    std::shared_ptr<Material> curMaterial;
    std::vector<std::array<double, 3>> vertices;
    std::vector<std::array<unsigned int, 3>> triangles;
    

    ObjReader(const std::string& filename) : cmap(cmap) {
        file.open(filename);
        if (!file.is_open()) {
            std::cerr << "Erro ao abrir o arquivo: " << filename << std::endl;
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
                    Color(mtlProps.color.r, mtlProps.color.g, mtlProps.color.b),
                    mtlProps.ka, mtlProps.ks, mtlProps.ke,
                    mtlProps.ns, mtlProps.ni, mtlProps.d
                );
            } else if (prefix == "v") {
                double x, y, z;
                iss >> x >> y >> z;
                vertices.push_back({x,y,z});
            } else if (prefix == "f") {
                unsigned int vi[3], ni[3];
                char slash;
                int _; // for skipping texture indices
                for (int i = 0; i < 3; ++i) {
                    iss >> vi[i] >> slash >> _ >> slash >> ni[i];
                    --vi[i];
                    --ni[i];
                }
                triangles.push_back({vi[0], vi[1], vi[2]});
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
    //         std::cout << "(" << tri.getPoint1().x << ", " << tri.getPoint1().y << ", " << tri.getPoint1().z << ") ";
    //         std::cout << "(" << tri.getPoint2().x << ", " << tri.getPoint2().y << ", " << tri.getPoint2().z << ") ";
    //         std::cout << "(" << tri.getPoint3().x << ", " << tri.getPoint3().y << ", " << tri.getPoint3().z << ") ";
    //         std::cout << std::flush;
    //         std::clog << std::endl;
    //     }
    // }
};

} // namespace Prism

#endif // PRISM_OBJREADER_HPP_
