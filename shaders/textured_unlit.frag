#version 330 core

in vec2 texCoord;

uniform sampler2D textureUnitID;
uniform vec3 objectColor;

out vec4 fragColor;

void main()
{
    vec4 texel = texture(textureUnitID, texCoord);

    if (texel.a < 0.1)
    {
        discard;
    }

    fragColor = vec4(texel.rgb * objectColor, texel.a);
}
