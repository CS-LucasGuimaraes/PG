#ifndef PRISM_MATERIAL_HPP_
#define PRISM_MATERIAL_HPP_

#include "prism_export.h"
#include "Prism/color.hpp"

namespace Prism {

    class PRISM_EXPORT Material {
        public:

        Material(Color color = Color(),  Vector3 ka = Vector3(), Vector3 ks = Vector3(), Vector3 ke = Vector3(), double ns = 0, double ni = 0 , double d = 0) : 
            color(color), ka(ka), ks(ks), ke(ke), ns(ns), ni(ni), d(d) {}

            Color color;
            Vector3 ka;
            Vector3 ks;
            Vector3 ke;
            double ns;
            double ni;
            double d;


    };

}  // namespace Prism

#endif  // PRISM_MATERIAL_HPP_