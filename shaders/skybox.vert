#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 sampleDirection;

void main()
{
    sampleDirection = position;

    // Keep rotation and scale, but center the skybox on the camera.
    vec3 skyPosition = mat3(modelMatrix) * position;
    mat4 skyView = mat4(mat3(viewMatrix));
    vec4 clipPosition = projectionMatrix * skyView * vec4(skyPosition, 1.0);

    // Far-plane depth: draw with GL_LEQUAL and depth writes disabled.
    gl_Position = clipPosition.xyww;
}
