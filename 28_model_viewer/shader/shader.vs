#version 330

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextCoord;

layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBiTangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 N;
out vec3 B;
out vec3 T;
out vec2 TextCoord;

void main()
{
    mat3 normalMatrix = mat3(transpose(inverse(model)));

    FragPos = vec3(model * vec4(aPos , 1.0));
    N = normalMatrix * aNormal;
    B = normalMatrix * aBiTangent;
    T = normalMatrix * aTangent;
    TextCoord = aTextCoord;

    gl_Position = projection * view * model * vec4(aPos , 1.0);
}