#version 330 core

in vec3 worldPosition;
in vec3 vertexColor;
in vec3 worldNormal;

uniform vec3 cameraPosition;
uniform vec3 lightPosition = vec3(0.0);
uniform vec3 lightColor = vec3(1.0);
uniform vec3 objectColor = vec3(0.385, 0.647, 0.812);

uniform float ambientStrength = 0.08;
uniform float specularStrength = 0.5;
uniform float shininess = 32.0;
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

vec3 calculateAmbient(vec3 surfaceColor)
{
    return ambientStrength * surfaceColor;
}

vec3 calculateDiffuse(float diffuseFactor, vec3 surfaceColor)
{
    return diffuseFactor * surfaceColor * lightColor;
}

float calculateSpecularFactor(vec3 normal, vec3 lightDirection, vec3 viewDirection, float diffuseFactor)
{
    // A surface facing away from the light receives no specular highlight.
    if (diffuseFactor <= 0.0)
    {
        return 0.0;
    }

    vec3 reflectionDirection = reflect(-lightDirection, normal);
    return pow(max(dot(reflectionDirection, viewDirection), 0.0), shininess);
}

void main(void)
{
    vec3 normal = normalize(worldNormal);
    vec3 lightDirection = normalize(lightPosition - worldPosition);
    vec3 viewDirection = normalize(cameraPosition - worldPosition);
    vec3 surfaceColor = getSurfaceColor();

    float diffuseFactor = max(dot(normal, lightDirection), 0.0);
    float specularFactor = calculateSpecularFactor(normal, lightDirection, viewDirection, diffuseFactor);

    vec3 ambient = calculateAmbient(surfaceColor);
    vec3 diffuse = calculateDiffuse(diffuseFactor, surfaceColor);
    vec3 specular = specularStrength * specularFactor * lightColor;

    out_Color = vec4(ambient + diffuse + specular, 1.0);
}