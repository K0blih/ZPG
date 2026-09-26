#version 330 core

in vec3 vertexColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(vertexColor * vec3(0.15, 0.35, 1.0), 1.0);
}