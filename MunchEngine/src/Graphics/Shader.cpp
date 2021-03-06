#include "Graphics/Shader.h"
#include "Core/Common.h"
#include "Core/Logger.h"

#include <glm/gtc/type_ptr.hpp>

Shader::Shader() 
    : Resource(), id(0), uniforms(), tunits() {}

Shader::~Shader() {
    clear();
}

Shader* Shader::create() {
    id = glCreateProgram();
    if (!id) {
        Logger::error("Error creating shader program");
        error_exit(1);
    }
    return this;
}

Shader* Shader::addShaderFromPath(ShaderType shaderType, const std::string& path) {
    return addShaderFromSource(shaderType, readFile(path));
}

Shader* Shader::addShaderFromSource(ShaderType shaderType, const std::string& source) {
    GLenum shaderTypeEnum = static_cast<GLenum>(shaderType);
    GLuint shader = glCreateShader(shaderTypeEnum);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);
    GLint result = 0;
    GLchar eLog[1024] = { 0 };
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(shader, sizeof(eLog), 0, eLog);
        Logger::error("Error compiling shader :: ", eLog);
        error_exit(1);
    }
    glAttachShader(id, shader);
    return this;
}

void Shader::compile() {
    GLint result = 0;
    GLchar eLog[1024] = { 0 };
    glLinkProgram(id);
    glGetProgramiv(id, GL_LINK_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(id, sizeof(eLog), 0, eLog);
        Logger::error("Error linking program :: ", eLog);
        error_exit(1);
    }

    validate();
}

GLint Shader::getUniformLocation(const std::string& uniformName) {
    auto iter = uniforms.find(uniformName);
    if(iter == uniforms.end()) {
        GLint uniform = glGetUniformLocation(id, uniformName.c_str());
        iter = uniforms.insert({uniformName, uniform}).first;
    }
    return iter->second;
}

void Shader::seti1(const std::string& uniformName, GLint val) {
    GLint uniform = getUniformLocation(uniformName);
    seti1(uniform, val);
}

void Shader::setf1(const std::string& uniformName, GLfloat val) {
    GLint uniform = getUniformLocation(uniformName);
    setf1(uniform, val);
}

void Shader::setf2(const std::string& uniformName, GLfloat val1, GLfloat val2) {
    GLint uniform = getUniformLocation(uniformName);
    setf2(uniform, val1, val2);
}

void Shader::setf2(const std::string& uniformName, const glm::vec2& val) {
    GLint uniform = getUniformLocation(uniformName);
    setf2(uniform, val.x, val.y);
}

void Shader::setf3(const std::string& uniformName, GLfloat val1, GLfloat val2, GLfloat val3) {
    GLint uniform = getUniformLocation(uniformName);
    setf3(uniform, val1, val2, val3);
}

void Shader::setf3(const std::string& uniformName, const glm::vec3& val) {
    GLint uniform = getUniformLocation(uniformName);
    setf3(uniform, val);
}

void Shader::setm4(const std::string& uniformName, const glm::mat4& val) {
    GLint uniform = getUniformLocation(uniformName);
    setm4(uniform, val);
}

void Shader::settx(const std::string& uniformName, Texture& texture) {
    settx2d(uniformName, texture.id);
}

void Shader::settx2d(const std::string& uniformName, GLuint textureId) {
    auto pos = tunits.find(uniformName);
    if(pos == tunits.end()) {
        Logger::warn("Invalid texture assignment!");
        return;
    }
    unsigned tunit = pos->second;
    glActiveTexture(GL_TEXTURE0 + tunit);
    glBindTexture(GL_TEXTURE_2D, textureId);
}

void Shader::settx3d(const std::string& uniformName, GLuint textureId) {
    auto pos = tunits.find(uniformName);
    if(pos == tunits.end()) {
        Logger::warn("Invalid texture assignment!");
        return;
    }
    unsigned tunit = pos->second;
    glActiveTexture(GL_TEXTURE0 + tunit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
}

void Shader::seti1(GLint uniform, GLint val) {
    glUniform1i(uniform, val);
}

void Shader::setf1(GLint uniform, GLfloat val) {
    glUniform1f(uniform, val);
}

void Shader::setf2(GLint uniform, GLfloat val1, GLfloat val2) {
    glUniform2f(uniform, val1, val2);
}

void Shader::setf3(GLint uniform, GLfloat val1, GLfloat val2, GLfloat val3) {
    glUniform3f(uniform, val1, val2, val3);
}

void Shader::setf3(GLint uniform, const glm::vec3& val) {
    glUniform3f(uniform, val.x, val.y, val.z);
}

void Shader::setm4(GLint uniform, const glm::mat4& val) {
    glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(val));
}     

void Shader::use() {
    glUseProgram(id);
}

void Shader::clear() {
    if(copied) return;
    if (id) {
        glDeleteProgram(id);
    }
    decltype(uniforms)().swap(uniforms);
    id = 0;
}

void Shader::validate() {
    GLint result = 0;
    GLchar eLog[1024] = { 0 };
    glValidateProgram(id);
    glGetProgramiv(id, GL_VALIDATE_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(id, sizeof(eLog), 0, eLog);
        Logger::error("Error validating program :: ", eLog);
        error_exit(1);
    }
}
