#ifndef PRISM_COLOR_HPP_
#define PRISM_COLOR_HPP_

#include "prism_export.h"

namespace Prism {
    class PRISM_EXPORT Color {
        public:
            Color();
            Color(float red, float green, float blue);
            Color(const Color& other);
            float r, g, b;
    };

} // namespace Prism
#endif // PRISM_COLOR_HPP_