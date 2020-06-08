#include "UI/UIMesh.h"
#include "Core/Common.h"

UIMesh::UIMesh() 
    : Resource(), vao(), vbo(), ibo(), indexCount() {
}

UIMesh::~UIMesh() {
    clear();
}

void UIMesh::create(const GLfloat* vertices, const unsigned* indices, size_t numvertices, size_t numindices) {
    indexCount = numindices;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * numindices, indices, GL_STATIC_DRAW);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numvertices, vertices, GL_STATIC_DRAW); // or GL_DYNAMIC_DRAW

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 5, 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 5, (void*)(sizeof(vertices[0]) * 3));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind vbo
    glBindVertexArray(0); // unbind vao
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbind ibo
}

void UIMesh::createPlane() {
    static const GLfloat vertices[] = {
        0.f, 0.f, 0.f,  0.f, 0.f,
        0.f, 1.f, 0.f,  0.f, 1.f,
        1.f, 1.f, 0.f,  1.f, 1.f,
        1.f, 0.f, 0.f,  1.f, 0.f,
    };
    static const unsigned indices[] = {
        0, 1, 2, 2, 0, 3
    };
    create(vertices, indices, size(vertices), size(indices));
}

void UIMesh::createCircle() {

}

void UIMesh::render() const {
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbind ibo
    glBindVertexArray(0); // unbind vao
}

void UIMesh::clear() {
    if(copied) return;
    if (ibo) {
        glDeleteBuffers(1, &ibo);
    }
    if (vbo) {
        glDeleteBuffers(1, &vbo);
    }
    if (vao) {
        glDeleteBuffers(1, &vao);
    }
    ibo = vbo = vao = indexCount = 0;
}
