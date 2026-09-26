#version 330 core

in vec3 vertexColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(vertexColor * vec3(1.0, 0.15, 0.15), 1.0);
}