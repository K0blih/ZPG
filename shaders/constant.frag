#version 330 core

in vec3 vertexColor;

uniform vec3 objectColor = vec3(1.0);
uniform float useVertexColor = 0.0;

out vec4 out_Color;

vec3 getSurfaceColor()
{
    if (useVertexColor > 0.5)
    {
        return vertexColor;
    }

    return objectColor;
}

void main(void)
{
    out_Color = vec4(getSurfaceColor(), 1.0);
}
