#pragma once

#include "Graphics/Resource.h"

#include <GL/glew.h>

struct UIMesh : Resource {
    UIMesh();
    ~UIMesh();

    ADD_SPECIAL_CLASS_FUNCTIONS(UIMesh);

    void create(const GLfloat* vertices, const unsigned* indices, size_t numvertices, size_t numindices);
    void createPlane();
    void createCircle();
    void render() const;
    void clear();

    GLuint vao, vbo, ibo;
    GLsizei indexCount;
};