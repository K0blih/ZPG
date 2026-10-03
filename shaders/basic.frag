#version 330 core

in vec3 vertexColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(abs(vertexColor), 1.0);
    //FragColor = vec4(0.2, 0.6, 1.0, 1.0);
}