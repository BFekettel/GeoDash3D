#version 460 core
uniform float ambientStrength;
uniform vec3 lightColor;
uniform vec3 lightPos;

in vec3 Normal; //INPUT INFO FROM VERT SHADER
in vec3 ourColor;
in vec3 FragPos;

out vec4 FragColor; //OUTPUT INFORMATION


void main() {
    //Calculate ambient colour
    vec3 ambient = ambientStrength * lightColor;

    //Calculate normals
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    //Calculate diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    //Resulting Light
    vec3 result = (ambient + diffuse) * ourColor;

    //Output Info
    FragColor = vec4(result, 1.0);

    }