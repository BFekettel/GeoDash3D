//
// Created by Ben on 1/09/2025.
//

#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Shader::Shader() : ID(0) {}



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

    /*
     * This function runs the shader programs and returns the results of the shaders
     */
}

void Shader::recompile() {
    // Load shader source code from files
    std::string vertexCode = loadShaderSource(vertexSrcPath);
    std::string fragmentCode = loadShaderSource(fragmentSrcPath);

    if (vertexCode == "ERROR" || fragmentCode == "ERROR") {
        std::cerr << "Shader source loading failed!" << std::endl;
        return;
    }

    const char* vSrc = vertexCode.c_str();
    const char* fSrc = fragmentCode.c_str();

    // Compile shaders
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vSrc);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fSrc);

    // Create program
    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    // Check linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, nullptr, infoLog);
        std::cerr << "Shader linking failed: " << infoLog << std::endl;
    } else {
        std::cout << "Shader linked successfully." << std::endl;
    }

    // Delete shaders after linking
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}



void Shader::setMat4(const std::string& name, const glm::mat4& mat) const {
    int loc = glGetUniformLocation(ID, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setVec3(const std::string& name, const glm::vec3& vec) const {
    int loc = glGetUniformLocation(ID, name.c_str());
    glUniform3fv(loc, 1, &vec[0]);
}

void Shader::setFloat(const std::string &name, float val) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), val);
}

void Shader::setInt(const std::string &name, int val) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), val);
}

std::string Shader::loadShaderSource(const std::string &filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open shader file: " << filePath << std::endl;
        return "ERROR";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
