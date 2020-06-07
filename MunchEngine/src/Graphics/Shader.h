#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Resource.h"

#include <string>
#include <unordered_map>

struct Shader : Resource {
    Shader();
    ~Shader();

    ADD_SPECIAL_CLASS_FUNCTIONS(Shader);

    void createSrc(const char* vertSrc, const char* fragSrc);
    void createSrc(const char* vertSrc, const char* geomSrc, const char* fragSrc);
    void create(const char* vertPath, const char* fragPath);
    void create(const char* vertPath, const char* geomPath, const char* fragPath);

    GLint getUniformLocation(const std::string& uniformName);
    void seti1(const std::string& uniformName, GLint val);
    void setf1(const std::string& uniformName, GLfloat val);
    void setf3(const std::string& uniformName, GLfloat val1, GLfloat val2, GLfloat val3);
    void setf3(const std::string& uniformName, const glm::vec3& val);
    void setm4(const std::string& uniformName, const glm::mat4& val);
    static void seti1(GLint uniform, GLint val);
    static void setf1(GLint uniform, GLfloat val);
    static void setf3(GLint uniform, GLfloat val1, GLfloat val2, GLfloat val3);
    static void setf3(GLint uniform, const glm::vec3& val);
    static void setm4(GLint uniform, const glm::mat4& val);

    void use();
    void clear();

    void compile(const char* vertSrc, const char* fragSrc);
    void compile(const char* vertSrc, const char* geomSrc, const char* fragSrc);
    void validate();
    void add(const char* src, GLuint shaderType);
    
    GLuint id;
    std::unordered_map<std::string, GLint> uniforms;
};