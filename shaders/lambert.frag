#version 330 core

in vec3 worldPosition;
in vec3 worldNormal;

uniform vec3 lightPosition = vec3(0.0);
uniform vec3 lightColor = vec3(1.0);
uniform vec3 objectColor = vec3(0.385, 0.647, 0.812);

uniform float ambientStrength = 0.08;
uniform float attenuationConstant = 1.0;
uniform float attenuationLinear = 0.09;
uniform float attenuationQuadratic = 0.032;

out vec4 out_Color;

float calculateAttenuation(float distanceToLight)
{
    float attenuation = 1.0 / (attenuationConstant + attenuationLinear * distanceToLight + attenuationQuadratic * distanceToLight * distanceToLight);
    return clamp(attenuation, 0.0, 1.0);
}

vec3 calculateAmbient(vec3 surfaceColor)
{
    return ambientStrength * surfaceColor;
}

vec3 calculateDiffuse(vec3 normal, vec3 lightDirection, vec3 surfaceColor)
{
    float diffuseFactor = max(dot(normal, lightDirection), 0.0);
    return diffuseFactor * surfaceColor * lightColor;
}

void main(void)
{
    vec3 normal = normalize(worldNormal);
    vec3 lightDirection = normalize(lightPosition - worldPosition);
    float distanceToLight = length(lightPosition - worldPosition);
    float attenuation = calculateAttenuation(distanceToLight);

    vec3 ambient = calculateAmbient(objectColor);
    vec3 diffuse = calculateDiffuse(normal, lightDirection, objectColor);

    out_Color = vec4(ambient + attenuation * diffuse, 1.0);
}
