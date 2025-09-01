#version 330 core
in vec3 ourColor; //INPUT INFORMATION
out vec4 FragColor; //OUTPUT INFORMATION
void main() {
    FragColor = vec4(ourColor, 1.0);
    }