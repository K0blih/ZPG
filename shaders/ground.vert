#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 vertexColor;
out vec3 worldNormal;

void main()
{
    vertexColor = vec3(0.30, 0.21, 0.12);

    mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
    worldNormal = normalMatrix * normal;

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
}