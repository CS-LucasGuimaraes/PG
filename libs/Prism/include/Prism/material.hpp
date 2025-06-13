#ifndef PRISM_MATERIAL_HPP_
#define PRISM_MATERIAL_HPP_

#include "prism_export.h"
#include "Prism/color.hpp"

namespace Prism {

    class PRISM_EXPORT Material {
        public:
            Material(Color color) : color(color) {};

            Color color;
    };

}  // namespace Prism

#endif  // PRISM_MATERIAL_HPP_