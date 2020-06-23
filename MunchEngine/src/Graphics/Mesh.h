#pragma once

#include <GL/glew.h>

#include <string>

#include "Graphics/Resource.h"
#include "Graphics/Image.h"

struct Mesh : Resource {
    Mesh();
    ~Mesh();

    ADD_SPECIAL_CLASS_FUNCTIONS(Mesh);

    void create(const GLfloat* vertices, const unsigned* indices, size_t numvertices, size_t numindices);
    void create(const GLfloat* vertices, const GLfloat* tangents, const unsigned* indices, size_t numvertices, size_t numindices);
    void create(const std::string& path);
    void createCube();
    void createPlane();
    void createTerrain();
    void createTerrain(const Image& heightMap);
    void createSphere();

    void render() const;
    void clear();

    GLuint vao, vbo, vbot, ibo;
    GLsizei indexCount;
    GLenum primitiveType;
};
