/**
 * @file Prism.hpp
 * @brief Prism Render Engine
 *
 * @copyright Copyright (c) 2025 src-lua
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifdef PRISM_BUILD_CORE
#include "Prism/core/color.hpp"
#include "Prism/core/material.hpp"
#include "Prism/core/matrix.hpp"
#include "Prism/core/point.hpp"
#include "Prism/core/ray.hpp"
#include "Prism/core/style.hpp"
#include "Prism/core/utils.hpp"
#include "Prism/core/vector.hpp"
#endif // PRISM_CORE

#ifdef PRISM_BUILD_OBJECTS
#include "Prism/objects/Colormap.hpp"
#include "Prism/objects/ObjReader.hpp"
#include "Prism/objects/mesh.hpp"
#include "Prism/objects/objects.hpp"
#include "Prism/objects/plane.hpp"
#include "Prism/objects/sphere.hpp"
#include "Prism/objects/triangle.hpp"
#endif // PRISM_BUILD_OBJECTS

#ifdef PRISM_BUILD_SCENE
#include "Prism/scene/camera.hpp"
#include "Prism/scene/scene.hpp"
#include "Prism/scene/scene_parser.hpp"
#endif