#version 460 core
uniform float ambientStrength;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform sampler2D texture1;

in vec3 Normal;
in vec3 ourColor;
in vec3 FragPos;
in vec2 TexCoord;

out vec4 FragColor;

void main() {
    // Ambient
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Combine with texture
    vec3 texColor = texture(texture1, TexCoord).rgb;
    vec3 result = (ambient + diffuse) * texColor * ourColor;

    FragColor = vec4(result, 1.0);
}
