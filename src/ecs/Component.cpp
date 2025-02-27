#include "include/ecs/Component.h"

namespace Engine {
namespace ECS {

// Implement virtual destructor to ensure proper cleanup of derived classes
Component::~Component() = default;

// Other methods were defined inline in the header

} // namespace ECS
} // namespace Engine
