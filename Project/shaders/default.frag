#version 460 core
#define MAX_LIGHTS 16

struct Material {
    sampler2D diffuse;
    sampler2D specularMap;
    sampler2D normalMap;
    vec3 ambient;
    float shininess;
};
uniform Material material;

struct Light {
    int type;            // 0 = directional, 1 = point, 2 = spot
    vec3 position;
    vec3 direction;
    vec3 color;
    float intensity;
    float radius;
    float cutOff;
    float outerCutOff;
};
uniform int numLights;
uniform Light lights[MAX_LIGHTS];

uniform vec3 viewPos;
uniform sampler2D shadowMap;

in vec3 FragPos;
in vec3 ourColor;
in vec2 TexCoord;
in mat3 TBN;
in vec4 FragPosLightSpace;

out vec4 FragColor;

// 🔹 Shadow calculation with PCF + slope-scaled bias
float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
    // Transform to [0,1] range
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    // If outside light frustum, skip
    if (projCoords.z > 1.0) return 0.0;

    // Bias reduces acne, scaled by angle between normal & light
    float bias = max(0.002 * (1.0 - dot(normal, lightDir)), 0.005);

    // Percentage Closer Filtering
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);

    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += (projCoords.z - bias > pcfDepth ? 1.0 : 0.0);
        }
    }
    shadow /= 9.0;

    return shadow;
}

vec3 CalcLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 fragPosLightSpace)
{
    vec3 lightDir;
    float distance = 1.0;

    if (light.type == 0) { // directional
                           lightDir = normalize(-light.direction);
    } else {
        vec3 lightVec = light.position - fragPos;
        distance = length(lightVec);
        lightDir = normalize(lightVec);
    }

    // Ambient
    vec3 ambient = material.ambient * light.color * light.intensity;

    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * light.color * light.intensity;

    // Specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specMap = texture(material.specularMap, TexCoord).rgb;
    vec3 specular = spec * specMap * light.color * light.intensity;

    // Attenuation
    float attenuation = 1.0;
    if (light.type == 1 || light.type == 2) {
        attenuation = clamp(1.0 - (distance / light.radius), 0.0, 1.0);
        attenuation *= attenuation;
    }

    // Spotlight factor
    float spotlightFactor = 1.0;
    if (light.type == 2) {
        float theta = dot(lightDir, normalize(-light.direction));
        float epsilon = max(light.cutOff - light.outerCutOff, 0.0001);
        spotlightFactor = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    }

    diffuse *= attenuation * spotlightFactor;
    specular *= attenuation * spotlightFactor;

    // 🔹 Apply shadow only for shadow-casting lights
    float shadow = 0.0;
    if (light.type == 0 || light.type == 2) {
        shadow = ShadowCalculation(fragPosLightSpace, normal, lightDir);
    }

    return ambient + (1.0 - shadow) * (diffuse + specular);
}

void main()
{
    // Normal mapping
    vec3 tangentNormal = texture(material.normalMap, TexCoord).rgb;
    tangentNormal = tangentNormal * 2.0 - 1.0;
    vec3 norm = normalize(TBN * tangentNormal);

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 totalLight = vec3(0.0);

    for (int i = 0; i < numLights; i++) {
        totalLight += CalcLight(lights[i], norm, FragPos, viewDir, FragPosLightSpace);
    }

    vec3 texColor = texture(material.diffuse, TexCoord).rgb;
    vec3 result = totalLight * texColor * ourColor;
    FragColor = vec4(result, 1.0);
}
