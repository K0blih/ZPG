#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 worldPosition;
out vec3 vertexColor;
out vec3 worldNormal;

vec3 transformNormalToWorld(vec3 modelNormal)
{
    // Inverse transpose preserves perpendicular normals under non-uniform scaling.
    mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
    return normalMatrix * modelNormal;
}

void main(void)
{
    vec4 world = modelMatrix * vec4(position, 1.0);
    worldPosition = world.xyz / world.w;

    worldNormal = transformNormalToWorld(normal);

    vertexColor = color;
    gl_Position = projectionMatrix * viewMatrix * world;
}
