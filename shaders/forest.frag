#version 330 core

in vec3 vertexColor;
in vec3 worldNormal;

out vec4 FragColor;

void main()
{
    vec3 normal = normalize(worldNormal);

    // This creates shades without introducing a light source.
    float shade = 0.3 + 0.7 * abs(normal.y);

    FragColor = vec4(shade * vertexColor, 1.0);
}