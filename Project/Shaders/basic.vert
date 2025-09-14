#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec2 aTexCoord;

out vec3 Normal;
out vec3 ourColor;
out vec3 FragPos;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    // Final clip-space position
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    // World-space fragment position
    FragPos = vec3(model * vec4(aPos, 1.0));

    // Vertex color passthrough
    ourColor = aColor;

    // Proper normal transform (normal matrix)
    Normal = normalize(mat3(transpose(inverse(model))) * aNormal);

    // Texture coordinate passthrough
    TexCoord = aTexCoord;
}
