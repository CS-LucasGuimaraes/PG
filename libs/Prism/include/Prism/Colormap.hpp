
#ifndef PRISM_COLORMAP_HPP_
#define PRISM_COLORMAP_HPP_

/*
Classe de leitura de arquivos .mtl, que guarda cores e propriedades de materiais.

A saber que:
    - kd = Difuso (Cor do objeto)
    - ks = Specular (Reflexivo)
    - ke = Emissivo 
    - ka = Ambiente
    - ns = Brilho
    - ni = Índice de refração
    - d = Opacidade

A classe precisa ser instânciada passando o caminho do arquivo .mtl correspondente
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include "Prism/vector.hpp"
#include "Prism/material.hpp"
#include "prism_export.h"
using namespace std;

namespace Prism{
class colormap {

public:
    map<string, Material> mp;

    //Construtor    
    colormap(){};
    colormap(string input){

        // construtor: lê arquivo cores.mtl e guarda valores RGB associados a cada nome

        std::ifstream mtlFile(input);

        if (!mtlFile.is_open()) {
            std::cerr << "erro abrindo arquivo cores.mtl\n";
        }

        string line, currentMaterial;

        while (std::getline(mtlFile, line)) {
            std::istringstream iss(line);
            std::string keyword;
            iss >> keyword;

            if (keyword == "newmtl") {
                iss >> currentMaterial;
                if (!currentMaterial.empty()) {
                    mp[currentMaterial] = Material();
                }
            } else if (keyword == "Kd") {
                double kdR, kdG, kdB;
                iss >> kdR >> kdG >> kdB;
                if (!currentMaterial.empty()) {
                    mp[currentMaterial].color = Color(kdR, kdG, kdB);
                }
            } else if (keyword == "Ks") {
                double ksR, ksG, ksB;
                iss >> ksR >> ksG >> ksB;
                if (!currentMaterial.empty()) {
                    mp[currentMaterial].ks = Vector3(ksR, ksG, ksB);
                }
            } else if (keyword == "Ke") {
                double keR, keG, keB;
                iss >> keR >> keG >> keB;
                if (!currentMaterial.empty()) {
                    mp[currentMaterial].ke = Vector3(keR, keG, keB);
                }
            } else if (keyword == "Ka") {
                double kaR, kaG, kaB;
                iss >> kaR >> kaG >> kaB;
                if (!currentMaterial.empty()) {
                    mp[currentMaterial].ka = Vector3(kaR, kaG, kaB);
                }
            } else if (keyword == "Ns") {
                iss >> mp[currentMaterial].ns;
            } else if (keyword == "Ni") {
                iss >> mp[currentMaterial].ni;
            } else if (keyword == "d") {
                iss >> mp[currentMaterial].d;
            }
        }

        mtlFile.close();
    }

    Vector3 getColor(string& s){
        if (mp.find(s) != mp.end()) {
            return Vector3(mp[s].color.r, mp[s].color.g, mp[s].color.b);
        } else {
            cerr << "Error: cor " << s << " indefinida no arquivo .mtl\n";
            return Vector3(0,0,0);
        }
    }

    Material getMaterial(string& s){
        if (mp.find(s) != mp.end()) {
            return mp[s];
        } else {
            cerr << "Error: Cor " << s << " indefinida no arquivo .mtl\n";
            return Material();
        }
    }

};
}
#endif
