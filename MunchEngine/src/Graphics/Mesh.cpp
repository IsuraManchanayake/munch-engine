#include "Graphics/Mesh.h"
#include "Core/Common.h"
#include "Core/Math.h"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>
#include <array>


Mesh::Mesh() 
    : Resource(), vao(), vbo(), ibo(), indexCount() {}

Mesh::~Mesh() {
    clear();
}

void Mesh::create(const GLfloat* vertices, const unsigned* indices, size_t numvertices, size_t numindices) {
    indexCount = numindices;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numindices, indices, GL_STATIC_DRAW);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numvertices, vertices, GL_STATIC_DRAW); // or GL_DYNAMIC_DRAW

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 3));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 5));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind vbo
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbind ibo
    glBindVertexArray(0); // unbind vao
}

void Mesh::createCube() {
    static const GLfloat vertices[] = {
    //    x    y    z     u    v    nx    ny    nz
        // y = 0
        0.f, 0.f, 0.f,  0.f, 0.f,  0.f, -1.f,  0.f,  // 0
        1.f, 0.f, 0.f,  1.f, 0.f,  0.f, -1.f,  0.f,  // 1
        1.f, 0.f, 1.f,  1.f, 1.f,  0.f, -1.f,  0.f,  // 2
        0.f, 0.f, 1.f,  0.f, 1.f,  0.f, -1.f,  0.f,  // 3
        // y = 1
        0.f, 1.f, 1.f,  0.f, 1.f,  0.f,  1.f,  0.f,  // 4
        1.f, 1.f, 1.f,  1.f, 1.f,  0.f,  1.f,  0.f,  // 5
        1.f, 1.f, 0.f,  1.f, 0.f,  0.f,  1.f,  0.f,  // 6
        0.f, 1.f, 0.f,  0.f, 0.f,  0.f,  1.f,  0.f,  // 7
        //z = 0
        0.f, 0.f, 0.f,  0.f, 0.f,  0.f,  0.f, -1.f,
        0.f, 1.f, 0.f,  0.f, 1.f,  0.f,  0.f, -1.f,
        1.f, 1.f, 0.f,  1.f, 1.f,  0.f,  0.f, -1.f,
        1.f, 0.f, 0.f,  1.f, 0.f,  0.f,  0.f, -1.f,
        // z = 1
        1.f, 0.f, 1.f,  1.f, 0.f,  0.f,  0.f,  1.f,
        1.f, 1.f, 1.f,  1.f, 1.f,  0.f,  0.f,  1.f,
        0.f, 1.f, 1.f,  0.f, 1.f,  0.f,  0.f,  1.f,
        0.f, 0.f, 1.f,  0.f, 0.f,  0.f,  0.f,  1.f,
        // x = 0
        0.f, 0.f, 0.f,  0.f, 0.f, -1.f,  0.f,  0.f,
        0.f, 0.f, 1.f,  0.f, 1.f, -1.f,  0.f,  0.f,
        0.f, 1.f, 1.f,  1.f, 1.f, -1.f,  0.f,  0.f,
        0.f, 1.f, 0.f,  1.f, 0.f, -1.f,  0.f,  0.f,
        // x = 1
        1.f, 1.f, 0.f,  1.f, 0.f,  1.f,  0.f,  0.f,
        1.f, 1.f, 1.f,  1.f, 1.f,  1.f,  0.f,  0.f,
        1.f, 0.f, 1.f,  0.f, 1.f,  1.f,  0.f,  0.f,
        1.f, 0.f, 0.f,  0.f, 0.f,  1.f,  0.f,  0.f
    };
    static const unsigned indices[] = {
         0,  1,  2,  2,  0,  3,
         4,  5,  6,  6,  4,  7,
         8,  9, 10, 10,  8, 11,
        12, 13, 14, 14, 12, 15,
        16, 17, 18, 18, 16, 19,
        20, 21, 22, 22, 20, 23
    };
    create(vertices, indices, size(vertices), size(indices));
}

void Mesh::createPlane() {
    static const GLfloat vertices[] = {
    //    x    y    z     u    v    nx   ny   nz
        // z = 0
        0.f, 0.f, 0.f,  0.f, 0.f,  0.f, 0.f, 1.f,
        0.f, 1.f, 0.f,  0.f, 1.f,  0.f, 0.f, 1.f,
        1.f, 1.f, 0.f,  1.f, 1.f,  0.f, 0.f, 1.f,
        1.f, 0.f, 0.f,  1.f, 0.f,  0.f, 0.f, 1.f,
    };
    static const unsigned indices[] = {
        0,  1,  2,  2,  0,  3,
    };
    create(vertices, indices, size(vertices), size(indices));
}


// void Mesh::createTerrain() {
//     static const unsigned subx = 50;
//     static const unsigned suby = 50;

//     static const GLfloat qv[] = {
//     //    x    y    z     u    v    nx   ny   nz
//         // z = 0
//         0.f, 0.f, 0.f,  0.f, 0.f,  0.f, 0.f, 1.f,
//         0.f, 1.f, 0.f,  0.f, 1.f,  0.f, 0.f, 1.f,
//         1.f, 1.f, 0.f,  1.f, 1.f,  0.f, 0.f, 1.f,
//         1.f, 0.f, 0.f,  1.f, 0.f,  0.f, 0.f, 1.f,
//     };
//     static const unsigned qi[] = {
//         0,  1,  2,  2,  0,  3,
//     };

//     std::vector<GLfloat> vertices;
//     std::vector<unsigned> indices;
//     for(unsigned yy = 0; yy < suby; yy++) {
//         for(unsigned xx = 0; xx < subx; xx++) {
//             float x = static_cast<float>(xx) / subx;
//             float y = static_cast<float>(yy) / suby;
//             for(unsigned v = 0; v < 4; v++) {
//                 unsigned s = v * 8;
//                 // coords
//                 float sx = x + qv[s + 0] / subx;
//                 float sy = y + qv[s + 1] / suby;
//                 float sz = qv[s + 2] + perlin2D(sx * 10, sy * 10);
//                 vertices.insert(vertices.end(), { sx, sy, sz });
//                 // uv
//                 float su = sx;
//                 float sv = sy;
//                 vertices.insert(vertices.end(), { su, sv });
//                 // norms
//                 float nx = qv[s + 5];
//                 float ny = qv[s + 6];
//                 float nz = qv[s + 7];
//                 vertices.insert(vertices.end(), { nx, ny, nz });
//             }
//             unsigned b = (xx + yy * subx) * 4;
//             for(unsigned i = 0; i < size(qi); i++) {
//                 indices.push_back(b + qi[i]);
//             }
//         }
//     }

//     create(vertices.data(), indices.data(), vertices.size(), indices.size());
// }

void Mesh::createTerrain() {
    static const unsigned subx = 100;
    static const unsigned suby = 100;

    static const GLfloat qv[] = {
    //    x    y    z     u    v    nx   ny   nz
        // z = 0
        0.f, 0.f, 0.f,  0.f, 0.f,  0.f, 0.f, 1.f,
        0.f, 1.f, 0.f,  0.f, 1.f,  0.f, 0.f, 1.f,
        1.f, 1.f, 0.f,  1.f, 1.f,  0.f, 0.f, 1.f,
        1.f, 1.f, 0.f,  1.f, 1.f,  0.f, 0.f, 1.f,
        1.f, 0.f, 0.f,  1.f, 0.f,  0.f, 0.f, 1.f,
        0.f, 0.f, 0.f,  0.f, 0.f,  0.f, 0.f, 1.f,
    };
    static const unsigned qi[] = {
        0,  1,  2,  3,  4,  5,
    };

    std::vector<GLfloat> vertices;
    std::vector<unsigned> indices;
    for(unsigned yy = 0; yy < suby; yy++) {
        for(unsigned xx = 0; xx < subx; xx++) {
            float x = static_cast<float>(xx) / subx;
            float y = static_cast<float>(yy) / suby;
            for(unsigned v = 0; v < 6; v++) {
                unsigned s = v * 8;
                // coords
                float sx = x + qv[s + 0] / subx;
                float sy = y + qv[s + 1] / suby;
                float sz = qv[s + 2] + perlin2D(sx * 10, sy * 10);
                vertices.insert(vertices.end(), { sx, sy, sz });
                // uv
                float su = sx;
                float sv = sy;
                vertices.insert(vertices.end(), { su, sv });
                // norms
                float nx = qv[s + 5];
                float ny = qv[s + 6];
                float nz = qv[s + 7];
                vertices.insert(vertices.end(), { nx, ny, nz });
            }
            for(unsigned tr = 0; tr < 2; tr++) {
                unsigned tr_ = tr * 3 * 8 + (xx + yy * subx) * size(qv);
                float px = vertices[tr_ + 8 * 0 + 0 ];
                float py = vertices[tr_ + 8 * 0 + 1 ];
                float pz = vertices[tr_ + 8 * 0 + 2 ];
                float qx = vertices[tr_ + 8 * 1 + 0 ];
                float qy = vertices[tr_ + 8 * 1 + 1 ];
                float qz = vertices[tr_ + 8 * 1 + 2 ];
                float rx = vertices[tr_ + 8 * 2 + 0 ];
                float ry = vertices[tr_ + 8 * 2 + 1 ];
                float rz = vertices[tr_ + 8 * 2 + 2 ];
                float ax = qx - px;
                float ay = qy - py;
                float az = qz - pz;
                float bx = rx - px;
                float by = ry - py;
                float bz = rz - pz;
                float nx = by * az - ay * bz;
                float ny = ax * bz - bx * az;
                float nz = bx * ay - ax * by;
                for(unsigned ni = 0; ni < 3; ni++) {
                    vertices[tr_ + 8 * ni + 5 + 0] = nx;
                    vertices[tr_ + 8 * ni + 5 + 1] = ny;
                    vertices[tr_ + 8 * ni + 5 + 2] = nz;
                }
            }
            unsigned b = (xx + yy * subx) * size(qi);
            for(unsigned i = 0; i < size(qi); i++) {
                indices.push_back(b + qi[i]);
            }
        }
    }

    create(vertices.data(), indices.data(), vertices.size(), indices.size());
}

void Mesh::createSphere() {
    static const unsigned lgs = 50;
    static const unsigned lts = 50;
    static const float pi = glm::pi<float>();
    std::vector<GLfloat> vertices;
    std::vector<unsigned> indices;
    auto vpb = [&vertices] (auto&&... args) {
        std::array<GLfloat, sizeof...(args)> temp = {args...};
        vertices.insert(vertices.end(), temp.begin(), temp.end());
    };
    auto ipb = [&indices] (auto&&... args) {
        std::array<GLfloat, sizeof...(args)> temp = {args...};
        indices.insert(indices.end(), temp.begin(), temp.end());
    };
    vpb(0, 0, 1, 0, 0, 0, 0, 1);
    for(size_t i = 1; i < lts; i++) {
        for(size_t j = 0; j <= lgs; j++) {
            float a1 = i * (pi / lts);
            float a2 = j * (pi * 2.f / lgs);
            float z = cos(a1);
            float r = sin(a1);
            float x = r * cos(a2);
            float y = r * sin(a2);
            vpb(x, y, z, 0, 0, x, y, z);
        }
    }
    vpb(0, 0, -1, 0, 0, 0, 0, -1);
    // top hull
    for(size_t i = 0; i < lgs; i++) {
        ipb(0, 1 + i, 2 + i);
    }
    // body
    for(size_t i = 0; i < lts - 2; i++) {
        for(size_t j = 0; j <= lgs; j++) {
            size_t idx1 = 1 + (lgs + 1) * i + j;
            size_t idx2 = 1 + (lgs + 1) * i + j + 1;
            size_t idx3 = 1 + (lgs + 1) * (i + 1) + j + 1;
            size_t idx4 = 1 + (lgs + 1) * (i + 1) + j;
            ipb(idx1, idx2, idx3);
            ipb(idx1, idx3, idx4);
        }
    }
    // bottom hull
    for(size_t i = 0; i < lgs; i++) {
        size_t idx1 = 1 + (lgs + 1) * (lts - 2) + i;
        size_t idx2 = 1 + (lgs + 1) * (lts - 2) + i + 1;
        size_t idx3 = 1 + (lgs + 1) * (lts - 1);
        ipb(idx2, idx1, idx3);
    }
    create(vertices.data(), indices.data(), vertices.size(), indices.size());

}

void Mesh::render() const {
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbind ibo
    glBindVertexArray(0); // unbind vao
}

void Mesh::clear() {
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