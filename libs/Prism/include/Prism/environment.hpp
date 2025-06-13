#ifndef PRISM_ENVIRONMENT_HPP_
#define PRISM_ENVIRONMENT_HPP_
#include "prism_export.h"


namespace Prism{
    class Color;

    class PRISM_EXPORT Ppm{
        public:
            Ppm(std::string file_name, size_t lines, size_t cols);
            ~Ppm();
            bool append_pixel(const Color& color);
        private:
            std::ofstream file;
            unsigned char convert_range(float f);
    }
    class PRISM_EXPORT Environment{

    }
}

#endif // PRISM_ENVIRONMENT_HPP_