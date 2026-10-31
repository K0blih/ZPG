#pragma once

#include <glm/vec3.hpp>

class ShaderProgram;

class Material {
public:
    Material() = default;

    void setColor(const glm::vec3& value);
    void setAmbientStrength(float value);
    void setSpecularStrength(float value);
    void setShininess(float value);
    void setUseVertexColor(bool value);

    void apply(ShaderProgram& shaderProgram) const;

private:
    glm::vec3 color{1.0f};

    float ambientStrength = 0.08f;
    float specularStrength = 0.5f;
    float shininess = 32.0f;

    bool useVertexColor = false;
};