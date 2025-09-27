#version 460 core
uniform vec3 lightColor;
uniform vec3 lightPos;       // world-space position of the light
uniform vec3 viewPos;        // camera position
uniform float lightIntensity;

in vec3 FragPos;
in vec3 ourColor;
in vec2 TexCoord;
in mat3 TBN;

out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specularMap;
    sampler2D normalMap;
    vec3 ambient;
    float shininess;
};
uniform Material material;

uniform float lightRadius;   // max reach of the spherical light
uniform float lightGradient; // extra falloff control

void main() {
    // --- Normal mapping (tangent → world space) ---
    vec3 tangentNormal = texture(material.normalMap, TexCoord).rgb;
    tangentNormal = tangentNormal * 2.0 - 1.0; // [0,1] → [-1,1]

    vec3 norm = normalize(TBN * tangentNormal);

    // --- Ambient ---
    vec3 ambient = material.ambient * lightColor * lightIntensity;

    // --- Light direction ---
    vec3 lightVec = lightPos - FragPos;
    float distance = length(lightVec);
    vec3 lightDir = normalize(lightVec);

    // --- Diffuse ---
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * lightIntensity;

    // --- Specular ---
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 specMap = texture(material.specularMap, TexCoord).rgb;
    vec3 specular = spec * specMap * lightColor * lightIntensity;

    // --- Attenuation (inverse square falloff) ---
    float attenuation = 1.0 / (distance * distance);
    if (distance > lightRadius)
    attenuation = 0.0;

    // Gradient = control for softer/sharper falloff
    attenuation = pow(attenuation, lightGradient);

    // Scale by radius factor (to balance brightness vs radius)
    attenuation *= (lightRadius * 0.1);

    diffuse *= attenuation;
    specular *= attenuation;

    // --- Texture sampling ---
    vec3 texColor = texture(material.diffuse, TexCoord).rgb;

    // --- Final result ---
    vec3 result = (ambient + diffuse) * texColor * ourColor + specular;
    FragColor = vec4(result, 1.0);
}
