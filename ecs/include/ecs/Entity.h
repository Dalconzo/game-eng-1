// Entity.h
#ifndef ENTITY_H
#define ENTITY_H

#include <cstdint>

// For now, an entity is just a unique ID.
// Future: Add versioning for entity reuse (e.g., to handle deletion and recycling).
using Entity = std::uint32_t;

#endif // ENTITY_H