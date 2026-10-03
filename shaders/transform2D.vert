#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

uniform vec3 translation;
uniform float scale;
uniform float rotationAngle;

out vec3 vertexColor;

void main()
{
    vec3 p = position * scale;

    float c = cos(rotationAngle);
    float s = sin(rotationAngle);

    vec3 rotated = vec3(
        p.x * c - p.y * s,
        p.x * s + p.y * c,
        p.z
    );

    vertexColor = color;
    gl_Position = vec4(rotated + translation, 1.0);
}