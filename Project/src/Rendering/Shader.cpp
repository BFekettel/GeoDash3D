//
// Created by Ben on 1/09/2025.
//

#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Shader::Shader(const char* vertexSrc, const char* fragmentSrc) {
    unsigned int vertex = compileShader(GL_VERTEX_SHADER, vertexSrc);
    unsigned int fragment = compileShader(GL_FRAGMENT_SHADER, fragmentSrc);

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    int success;
    char infoLog[512];
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, nullptr, infoLog);
        std::cerr << "Shader linking failed: " << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

unsigned int Shader::compileShader(unsigned int type, const char* source) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compile error: " << infoLog << std::endl;
    }
    return shader;
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const {
    int loc = glGetUniformLocation(ID, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setVec3(const std::string& name, const glm::vec3& vec) const {
    int loc = glGetUniformLocation(ID, name.c_str());
    glUniform3fv(loc, 1, &vec[0]);
}
