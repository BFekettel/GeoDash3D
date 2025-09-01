//
// Created by Ben on 1/09/2025.
//

#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Shader::Shader(const char* vertexSrc, const char* fragmentSrc) {
    unsigned int vertex = compileShader(GL_VERTEX_SHADER, vertexSrc);
    unsigned int fragment = compileShader(GL_FRAGMENT_SHADER, fragmentSrc);

    ID = glCreateProgram(); //Create program and assign ID
    /*
     * Vertex Shader adds in the shape of the object, plots the vertices and the connected polygons
     * Fragment Shader adds in the colour of the object
     */
    glAttachShader(ID, vertex); //Uses Vertex Shader Program
    glAttachShader(ID, fragment); //Uses Fragment Shader Program
    glLinkProgram(ID);

    int success;
    char infoLog[512];
    glGetProgramiv(ID, GL_LINK_STATUS, &success); //Sees if Shader programs worked
    if (!success) {
        glGetProgramInfoLog(ID, 512, nullptr, infoLog);
        std::cerr << "Shader linking failed: " << infoLog << std::endl;
    } else {
        std::cout << "Shader linked" << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    /* the goal of the above, attaching shaders and deleting it,
     * is to test to see if the shaders work.
     * if they do then we delete them, use them later,
    */
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

    /*
     * This function runs the shader programs and returns the results of the shaders
     */
}

void Shader::recompile(const char* vertexSrc, const char* fragmentSrc) {
    // Compile new shaders
    unsigned int vertex = compileShader(GL_VERTEX_SHADER, vertexSrc);
    unsigned int fragment = compileShader(GL_FRAGMENT_SHADER, fragmentSrc);

    // Create new program
    unsigned int newID = glCreateProgram();
    glAttachShader(newID, vertex);
    glAttachShader(newID, fragment);
    glLinkProgram(newID);

    int success;
    char infoLog[512];
    glGetProgramiv(newID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(newID, 512, nullptr, infoLog);
        std::cerr << "Shader re-linking failed: " << infoLog << std::endl;
    } else {
        std::cout << "Shader recompiled successfully" << std::endl;
    }

    // Delete old program and shaders
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if (ID != 0) glDeleteProgram(ID);

    // Update ID
    ID = newID;

}


void Shader::setMat4(const std::string& name, const glm::mat4& mat) const {
    int loc = glGetUniformLocation(ID, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setVec3(const std::string& name, const glm::vec3& vec) const {
    int loc = glGetUniformLocation(ID, name.c_str());
    glUniform3fv(loc, 1, &vec[0]);
}

void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}