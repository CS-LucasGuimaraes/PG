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
  private:
    std::ifstream file;
    std::vector<Point3> vertices;
    std::vector<Triangle> triangles;  
    Material curMaterial;
    colormap cmap;

  public:
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
                curMaterial = Material(
                    Color(mtlProps.color.r, mtlProps.color.g, mtlProps.color.b),
                    mtlProps.ka, mtlProps.ks, mtlProps.ke,
                    mtlProps.ns, mtlProps.ni, mtlProps.d
                );
            } else if (prefix == "v") {
                double x, y, z;
                iss >> x >> y >> z;
                vertices.emplace_back(x, y, z);
            } else if (prefix == "f") {
                int vi[3], ni[3];
                char slash;
                int _; // for skipping texture indices
                for (int i = 0; i < 3; ++i) {
                    iss >> vi[i] >> slash >> _ >> slash >> ni[i];
                    --vi[i];
                    --ni[i];
                }
                triangles.emplace_back(
                    vertices[vi[0]], vertices[vi[1]], vertices[vi[2]],
                    std::make_shared<Material>(curMaterial)
                );
            }
        }

        file.close();
    }

    /**
     * @brief Returns all parsed triangles from the OBJ file
     */
    std::vector<Triangle> getTriangles() const {
        return triangles;
    }

    /**
     * @brief Returns all parsed vertices from the OBJ file
     */
    std::vector<Point3> getVertices() const {
        return vertices;
    }

    /**
     * @brief Returns the last used material
     */
    Material getCurrentMaterial() const {
        return curMaterial;
    }

    /**
     * @brief Prints each triangle’s vertices to the console
     */
    void print_faces() const {
        int i = 0;
        for (const auto& tri : triangles) {
            std::clog << "Face " << (++i) << ": ";
            std::cout << "(" << tri.point1.x << ", " << tri.point1.y << ", " << tri.point1.z << ") ";
            std::cout << "(" << tri.point2.x << ", " << tri.point2.y << ", " << tri.point2.z << ") ";
            std::cout << "(" << tri.point3.x << ", " << tri.point3.y << ", " << tri.point3.z << ") ";
            std::cout << std::flush;
            std::clog << std::endl;
        }
    }
};

} // namespace Prism

#endif // PRISM_OBJREADER_HPP_
