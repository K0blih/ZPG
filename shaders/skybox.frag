#version 330 core

in vec3 sampleDirection;

uniform samplerCube textureUnitID;

out vec4 fragColor;

void main()
{
    fragColor = texture(textureUnitID, sampleDirection);
}
