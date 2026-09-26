#include "Scene.h"
#include "Shader.h"

#include <iterator>

// Include models
#include <sphere.h>

Model& Scene::addModel(const float* vertices, std::size_t floatCount, GLenum drawingMode)
{
    auto model = std::make_unique<Model>();
    model->create(vertices, floatCount, drawingMode);

    models.push_back(std::move(model));
    return *models.back();
}

ShaderProgram& Scene::addShaderProgram(const Shader& vertexShader, const Shader& fragmentShader)
{
    auto program = std::make_unique<ShaderProgram>();
    program->link(vertexShader, fragmentShader);

    shaderPrograms.push_back(std::move(program));
    return *shaderPrograms.back();
}

DrawableObject& Scene::addObject(Model& model, ShaderProgram& shaderProgram)
{
    objects.push_back(std::make_unique<DrawableObject>(model, shaderProgram));
    return *objects.back();
}

void Scene::create()
{
    // Create and compile the vertex and fragment shaders
    Shader vertex(GL_VERTEX_SHADER, "shaders/basic.vert");
    Shader basicFragment(GL_FRAGMENT_SHADER, "shaders/basic.frag");
    Shader redFragment(GL_FRAGMENT_SHADER, "shaders/red.frag");
    Shader blueFragment(GL_FRAGMENT_SHADER, "shaders/blue.frag");

    // Create and link the shader program 
    ShaderProgram& red = addShaderProgram(vertex, redFragment);
    ShaderProgram& blue = addShaderProgram(vertex, blueFragment);
    ShaderProgram& basic = addShaderProgram(vertex, basicFragment);

	float triangle[] = {
        -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,
        0.5f,  -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,
         0.0f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f
    };

    float square[] = {
        -0.5f, -0.5f, 0.0f,    1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,
        0.5f,  0.5f, 0.0f,    0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, 0.0f,    1.0f, 1.0f, 0.0f
	};

    Model& triangleMesh = addModel(
        triangle, std::size(triangle), GL_TRIANGLES
    );

    Model& squareMesh = addModel(
        square, std::size(square), GL_TRIANGLE_FAN
    );

    Model& sphereMesh = addModel(
        sphere, std::size(sphere), GL_TRIANGLES
    );

    addObject(triangleMesh, red);
	addObject(squareMesh, blue);
}

void Scene::draw()
{
    for (const auto& object : objects)
    {
        object->draw();
    }
}

void Scene::clear()
{
    objects.clear();
    models.clear();
    shaderPrograms.clear();
}

Scene::~Scene()
{
	clear();
}