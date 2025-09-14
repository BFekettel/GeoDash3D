#version 460 core
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float lightIntensity;

in vec3 Normal;
in vec3 ourColor;
in vec3 FragPos;
in vec2 TexCoord;

out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    vec3 ambient;
    vec3 specular;
    float shininess;
};
uniform Material material;

uniform float lightRadius;
uniform float lightGradient;

void main() {
    // Ambient from material
    vec3 ambient = material.ambient * lightColor * lightIntensity;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * lightIntensity;

    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spec * material.specular * lightColor * lightIntensity;

    // Distance-based attenuation (spherical)
    float distance = length(lightPos - FragPos);
    float attenuation = 0.0;
    if (distance < lightRadius) {
        float normalizedDist = distance / lightRadius;
        attenuation = mix(1.0, lightGradient, normalizedDist * normalizedDist);
        attenuation = clamp(attenuation, 0.0, 1.0);
    }
    diffuse *= attenuation;
    specular *= attenuation;

    // Texture sampling
    vec3 texColor = texture(material.diffuse, TexCoord).rgb;

    // Combine
    vec3 result = (ambient + diffuse) * texColor * ourColor + specular;
    FragColor = vec4(result, 1.0);
}
