#pragma once

#include "rendering/mesh.h"
#include <memory>

namespace engine {
namespace rendering {

class PrimitiveBuilder {
public:
    // Create basic primitives
    static std::unique_ptr<Mesh> createTriangle();
    static std::unique_ptr<Mesh> createCube();
    static std::unique_ptr<Mesh> createQuad();
};

} // namespace rendering
} // namespace engine
