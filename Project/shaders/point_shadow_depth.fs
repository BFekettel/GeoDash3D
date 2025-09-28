#version 460 core
in vec3 FragPosGS;   // from geometry shader
uniform vec3 lightPos;
uniform float farPlane;

out float FragDepth;

void main()
{
    float dist = length(FragPosGS - lightPos);
    FragDepth = dist / farPlane;  // store normalized depth
}
