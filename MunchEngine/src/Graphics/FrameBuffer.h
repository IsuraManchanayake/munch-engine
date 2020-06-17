#pragma once

#include <GL/glew.h>

#include "Graphics/Resource.h"

struct FrameBuffer : Resource {
    FrameBuffer();
    ~FrameBuffer();

    ADD_SPECIAL_CLASS_FUNCTIONS(FrameBuffer);

    void clear() override;
    void create(unsigned width, unsigned height);
    void write();
    void detach();

    unsigned width;
    unsigned height;
    unsigned fbo;
    unsigned textureId;
};