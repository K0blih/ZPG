#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform float time;

out vec3 vertexColor;

void main()
{
    vec3 pos = position;
    vertexColor = color;

    float period = 0.0;

    if (all(equal(color, vec3(1.0, 0.0, 0.0))))
    {
        // Seconds: one revolution per minute, stays red.
        period = 60.0;
    }
    else if (all(equal(color, vec3(0.0, 1.0, 0.0))))
    {
        // Minutes: one revolution per hour, display black.
        period = 3600.0;
        vertexColor = vec3(0.0);
    }
    else if (all(equal(color, vec3(0.0, 0.0, 1.0))))
    {
        // Hours: one revolution per 12 hours, display black.
        period = 43200.0;
        vertexColor = vec3(0.0);
    }
    else if (all(equal(color, vec3(1.0, 1.0, 0.0))))
    {
        // Logo: Pantone reference RGB from your image.
        vertexColor = vec3(5.0, 195.0, 222.0) / 255.0;
    }
    else if (all(equal(color, vec3(0.0))))
    {
        // Numbers, tick marks and rim.
        vertexColor = vec3(0.6);
    }

    if (period > 0.0)
    {
        float angle = 6.2831853 * mod(time, period) / period;

        float c = cos(angle);   
        float s = sin(angle);

        // Clockwise rotation around the model's origin.
        pos.xy = vec2(c * position.x + s * position.y, -s * position.x + c * position.y);
    }

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(pos, 1.0);
}