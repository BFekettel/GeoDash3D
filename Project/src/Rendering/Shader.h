#ifndef PROJECT_SHADER_H
#define PROJECT_SHADER_H

#include <string>
#include "glad/glad.h"
#include "glm/fwd.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
    unsigned int ID;

    Shader();
    void use() const { glUseProgram(ID); }

    // Compile functions
    void compile(const std::string& vertexPath, const std::string& fragmentPath);

    // Recompile with defaults
    void recompile();
    void recompile(const std::string& vertexPath, const std::string& fragmentPath);
    void recompile(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath);

    // Uniform helpers
    void setMat4(const std::string& name, const glm::mat4& mat) const;
    void setVec3(const std::string& name, const glm::vec3& vec) const;
    void setFloat(const std::string& name, float val) const;
    void setInt(const std::string& name, int val) const;

    static constexpr const char* vertexSrcPath   = "../shaders/default.vert";
    static constexpr const char* fragmentSrcPath = "../shaders/default.frag";

private:
    static unsigned int compileShader(unsigned int type, const char* source);
    std::string loadShaderSource(const std::string& filePath);
};

#endif // PROJECT_SHADER_H
