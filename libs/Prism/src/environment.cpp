#include "Prism/environment.hpp"
#include "Prism/color.hpp"
#include <fstream>
#include <string>
#include <cmath>

namespace Prism{
    Ppm::Ppm(std::string file_name, size_t lines, size_t cols){
        std::string path = "../data/outputs/"+file_name+".ppm";
        file.open(path,std::ios::app);
        if(file.is_open()){
            //configure size of ppm
            file << "P3" << std::endl;
            file << lines << " " << cols << std::endl;
        }
    }
    Ppm::~Ppm(){
        file.close();
    }
    bool Ppm::append_pixel(const Color& color){
        if(!file.is_open()){
            return false;
        }else{
            file << "   ";
            file << convert_range(color.r) << " " << convert_range(color.g) << " " << convert_range(color.b);
            return true;
        }
    }
    unsigned char Ppm::convert_range(float f) {
        return (unsigned char)std::floor(f*255);
    }
    
}