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
    if (gl_VertexID > 36443) { 
        vertexColor = vec3(0.12, 0.60, 0.08);
    }
    else {
        vertexColor = vec3(0.35, 0.18, 0.07); // Brown
    }

    // Transform normals correctly, including when scaling is used.
    mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
    worldNormal = normalMatrix * normal;

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
}