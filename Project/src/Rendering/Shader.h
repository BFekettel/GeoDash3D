//
// Created by Ben on 1/09/2025.
//

#ifndef PROJECT_SHADER_H
#define PROJECT_SHADER_H
#include <string>

#include "glad/glad.h"
#include "glm/fwd.hpp"


class Shader {
public:
    unsigned int ID;

    Shader(const char* vertexSrc, const char* fragmentSrc);
    void use() const { glUseProgram(ID); }

    void recompile(const char* vertexSrc, const char* fragmentSrc);

    void setMat4(const std::string& name, const glm::mat4& mat) const;
    void setVec3(const std::string& name, const glm::vec3& vec) const;
    void setInt(const std::string &name, int value) const;

    void setTexture(const std::string &name, GLuint textureUnit) const;

private:
    static unsigned int compileShader(unsigned int type, const char* source);
};


#endif //PROJECT_SHADER_H