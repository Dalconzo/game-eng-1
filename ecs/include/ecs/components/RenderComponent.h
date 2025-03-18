#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H

#include <string>

struct RenderComponent {
    unsigned int meshId;
    unsigned int textureId;
    std::string meshName;
};

#endif // RENDER_COMPONENT_H