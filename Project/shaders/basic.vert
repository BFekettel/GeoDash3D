#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec2 aTexCoord;
layout(location = 4) in vec3 aTangent;
layout(location = 5) in vec3 aBitangent;

out vec3 FragPos;
out vec3 ourColor;
out vec2 TexCoord;
out mat3 TBN;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    // Clip space
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    // World-space fragment position
    FragPos = vec3(model * vec4(aPos, 1.0));

    // Vertex color passthrough
    ourColor = aColor;

    // Texture coords passthrough
    TexCoord = aTexCoord;

    // --- Build TBN matrix ---
    mat3 normalMatrix = transpose(inverse(mat3(model)));

    vec3 T = normalize(normalMatrix * aTangent);
    vec3 B = normalize(normalMatrix * aBitangent);
    vec3 N = normalize(normalMatrix * aNormal);

    TBN = mat3(T, B, N);
}
