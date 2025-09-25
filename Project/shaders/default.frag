#version 460 core

#define MAX_LIGHTS 16

// --- Material ---
struct Material {
    sampler2D diffuse;
    sampler2D specularMap;
    sampler2D normalMap;
    vec3 ambient;
    float shininess;
};
uniform Material material;

// --- Light ---
struct Light {
    int type;              // 0 = global, 1 = point, 2 = spot
    vec3 position;
    vec3 direction;
    vec3 color;
    float intensity;
    float radius;          // only used for point & spot
    float cutOff;          // inner cutoff (spot only)
    float outerCutOff;     // outer cutoff (spot only)
};

uniform int numLights;
uniform Light lights[MAX_LIGHTS];

uniform vec3 viewPos;

// --- Inputs from vertex shader ---
in vec3 FragPos;
in vec3 ourColor;
in vec2 TexCoord;
in mat3 TBN;

out vec4 FragColor;

// ==============================
// Per-light calculation function
// ==============================
vec3 CalcLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir;
    float distance = 1.0;

    if (light.type == 0) {
        // Global / directional light
        lightDir = normalize(-light.direction);
    } else {
        // Point / Spot
        vec3 lightVec = light.position - fragPos;
        distance = length(lightVec);
        lightDir = normalize(lightVec);
    }

    // --- Ambient ---
    vec3 ambient = material.ambient * light.color * light.intensity;

    // --- Diffuse ---
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * light.color * light.intensity;

    // --- Specular ---
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specMap = texture(material.specularMap, TexCoord).rgb;
    vec3 specular = spec * specMap * light.color * light.intensity;

    // --- Attenuation for point/spot ---
    float attenuation = 1.0;
    if (light.type == 1 || light.type == 2) {
        attenuation = clamp(1.0 - (distance / light.radius), 0.0, 1.0);
        attenuation *= attenuation; // quadratic falloff
    }

    // --- Spotlight cone factor ---
    float spotlightFactor = 1.0;
    if (light.type == 2) {
        float theta = dot(lightDir, normalize(-light.direction));
        float epsilon = max(light.cutOff - light.outerCutOff, 0.0001); // avoid div/0
        spotlightFactor = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    }

    // Apply attenuation & spotlight
    diffuse *= attenuation * spotlightFactor;
    specular *= attenuation * spotlightFactor;

    return ambient + diffuse + specular;
}

// ==============================
// Main
// ==============================
void main() {
    // --- Normal mapping (tangent → world space) ---
    vec3 tangentNormal = texture(material.normalMap, TexCoord).rgb;
    tangentNormal = tangentNormal * 2.0 - 1.0;
    vec3 norm = normalize(TBN * tangentNormal);

    // --- View direction ---
    vec3 viewDir = normalize(viewPos - FragPos);

    // --- Accumulate lighting from all lights ---
    vec3 totalLight = vec3(0.0);
    for (int i = 0; i < numLights; i++) {
        totalLight += CalcLight(lights[i], norm, FragPos, viewDir);
    }

    // --- Texture sampling ---
    vec3 texColor = texture(material.diffuse, TexCoord).rgb;

    // --- Final output ---
    vec3 result = (totalLight * texColor * ourColor);
    FragColor = vec4(result, 1.0);
}
