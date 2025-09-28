#version 460 core
layout (location = 0) in vec3 aPos;

out vec3 FragPos;  // send world-space position to geometry shader

uniform mat4 model;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
}
