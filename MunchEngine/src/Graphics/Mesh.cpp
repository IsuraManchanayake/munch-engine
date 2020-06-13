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
    : Resource(), vao(), vbo(), vbot(), ibo(), indexCount() {}

Mesh::~Mesh() {
    clear();
}

void Mesh::create(const GLfloat* vertices, const unsigned* indices, size_t numvertices, size_t numindices) {
    indexCount = numindices;

    std::vector<GLfloat> tangents(3 * (numvertices / 8));
    for(unsigned idx = 0; idx < numindices; idx += 3) {
        unsigned i = indices[idx + 0];
        unsigned j = indices[idx + 1];
        unsigned k = indices[idx + 2];

        float x1 = vertices[8 * i + 0];
        float y1 = vertices[8 * i + 1];
        float z1 = vertices[8 * i + 2];
        float x2 = vertices[8 * j + 0];
        float y2 = vertices[8 * j + 1];
        float z2 = vertices[8 * j + 2];
        float x3 = vertices[8 * k + 0];
        float y3 = vertices[8 * k + 1];
        float z3 = vertices[8 * k + 2];

        float u1 = vertices[8 * i + 3];
        float v1 = vertices[8 * i + 4];
        float u2 = vertices[8 * j + 3];
        float v2 = vertices[8 * j + 4];
        float u3 = vertices[8 * k + 3];
        float v3 = vertices[8 * k + 4];

        float t1x, t1y, t1z;
        float t2x, t2y, t2z;
        float t3x, t3y, t3z;        
        {
            float du1 = u2 - u1; float dv1 = v2 - v1;
            float du2 = u3 - u1; float dv2 = v3 - v1;
            float e1x = x2 - x1; float e1y = y2 - y1; float e1z = z2 - z1; 
            float e2x = x3 - x1; float e2y = y3 - y1; float e2z = z3 - z1;
            float f = 1 / (du1 * dv2 - du2 * dv1);
            t1x = f * (dv2 * e1x - dv1 * e2x); t1y = f * (dv2 * e1y - dv1 * e2y); t1z = f * (dv2 * e1z - dv1 * e2z);
        }
        {
            float du1 = u1 - u2; float dv1 = v1 - v2;
            float du2 = u3 - u2; float dv2 = v3 - v2;
            float e1x = x1 - x2; float e1y = y1 - y2; float e1z = z1 - z2; 
            float e2x = x3 - x2; float e2y = y3 - y2; float e2z = z3 - z2;
            float f = 1 / (du1 * dv2 - du2 * dv1);
            t2x = f * (dv2 * e1x - dv1 * e2x); t2y = f * (dv2 * e1y - dv1 * e2y); t2z = f * (dv2 * e1z - dv1 * e2z);
        }
        {
            float du1 = u2 - u3; float dv1 = v2 - v3;
            float du2 = u1 - u3; float dv2 = v1 - v3;
            float e1x = x2 - x3; float e1y = y2 - y3; float e1z = z2 - z3; 
            float e2x = x1 - x3; float e2y = y1 - y3; float e2z = z1 - z3;
            float f = 1 / (du1 * dv2 - du2 * dv1);
            t3x = f * (dv2 * e1x - dv1 * e2x); t3y = f * (dv2 * e1y - dv1 * e2y); t3z = f * (dv2 * e1z - dv1 * e2z);
        }
        tangents[3 * i + 0] = t1x;
        tangents[3 * i + 1] = t1y;
        tangents[3 * i + 2] = t1z;
        tangents[3 * j + 0] = t2x;
        tangents[3 * j + 1] = t2y;
        tangents[3 * j + 2] = t2z;
        tangents[3 * k + 0] = t3x;
        tangents[3 * k + 1] = t3y;
        tangents[3 * k + 2] = t3z;
    }
    create(vertices, tangents.data(), indices, numvertices, numindices);
}

void Mesh::create(const GLfloat* vertices, const GLfloat* tangents, const unsigned* indices, size_t numvertices, size_t numindices) {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numindices, indices, GL_STATIC_DRAW);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numvertices, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, NULL);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 3));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 5));
    glEnableVertexAttribArray(2);

    glGenBuffers(1, &vbot);
    glBindBuffer(GL_ARRAY_BUFFER, vbot);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tangents[0]) * 3 * (numvertices / 8), tangents, GL_STATIC_DRAW);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(3);

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

void Mesh::createTerrain() {
    static const unsigned subx = 100;
    static const unsigned suby = 100;
    static const float perlinFreq = 6.f;
    static const float height = 3.f;

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
                float sz = qv[s + 2] + height * perlin2D(sx * perlinFreq + 1, sy * perlinFreq + 1);
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
        vertices.insert(vertices.end(), {GLfloat(args)...});
    }; 
    auto ipb = [&indices] (auto&&... args) {
        indices.insert(indices.end(), {unsigned(args)...});
    };
    for(size_t i = 0; i <= lgs; i++) {
        vpb(0, 0, 1, i * (1.f / lgs), 0, 0, 0, 1);
    }
    static const float alpha = 0.001f * 2;
    for(size_t i = 1; i < lts; i++) {
        for(size_t j = 0; j <= lgs; j++) {
            // float a1 = i * (pi / lts);
            float a1 = alpha / 2 + (i - 1) * ((pi - alpha) / (lts - 2));
            float a2 = j * ((pi * 2.f) / lgs);
            float z = cos(a1);
            float r = sin(a1);
            float x = r * cos(a2);
            float y = r * sin(a2);
            float u = a2 / (2.f * pi);
            float v = a1 / pi;
            vpb(x, y, z, u, v, x, y, z);
        }
    }
    for(size_t i = 0; i <= lgs; i++) {
        vpb(0, 0, -1, i * (1.f / lgs), 1, 0, 0, -1);
    }
    // top hull
    // for(size_t i = 0; i < lgs; i++) {
    //     ipb(i, 1 + i + lgs, 2 + i + lgs);
    // }
    // body
    for(size_t i = 0; i < lts - 2; i++) {
        for(size_t j = 0; j < lgs; j++) {
            size_t idx1 = lgs + 1 + (lgs + 1) * i + j;
            size_t idx2 = lgs + 1 + (lgs + 1) * i + j + 1;
            size_t idx3 = lgs + 1 + (lgs + 1) * (i + 1) + j + 1;
            size_t idx4 = lgs + 1 + (lgs + 1) * (i + 1) + j;
            ipb(idx1, idx2, idx3);
            ipb(idx1, idx3, idx4);
        }
    }
    // bottom hull
    // for(size_t i = 0; i < lgs; i++) {
    //     size_t idx1 = lgs + 1 + (lgs + 1) * (lts - 2) + i;
    //     size_t idx2 = lgs + 1 + (lgs + 1) * (lts - 2) + i + 1;
    //     size_t idx3 = lgs + 1 + (lgs + 1) * (lts - 1) + i;
    //     ipb(idx2, idx1, idx3);
    // }
    // create(vertices.data(), tangents.data(), indices.data(), vertices.size(), indices.size());
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
    if (vbot) {
        glDeleteBuffers(1, &vbot);
    }
    if (vao) {
        glDeleteBuffers(1, &vao);
    }
    ibo = vbo = vao = indexCount = 0;
}