#include "Scene.h"
#include "Shader.h"

#include <iterator>

// Include models
#include <sphere.h>
#include <bushes.h>
#include <tree.h>
#include <CHO0289.h>

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

void Scene::createSignature()
{
    clear();

    // Create and compile the vertex and fragment shaders
    Shader vertex(GL_VERTEX_SHADER, "shaders/transform3D.vert");
    Shader fragment(GL_FRAGMENT_SHADER, "shaders/basic.frag");

    // Create and link the shader program 
    ShaderProgram& program = addShaderProgram(vertex, fragment);

    Model& mesh = addModel(cho0289, std::size(cho0289), GL_TRIANGLES);

    DrawableObject& login = addObject(mesh, program);

    login.setScale(0.15f);
    login.setTranslation(0.83f, -0.95f, 0.0f);
}

void Scene::createTriangle()
{
    clear();

    // Create and compile the vertex and fragment shaders
    Shader vertex(GL_VERTEX_SHADER, "shaders/transform3D.vert");
    Shader basicFragment(GL_FRAGMENT_SHADER, "shaders/basic.frag");

    // Create and link the shader program 
    ShaderProgram& basic = addShaderProgram(vertex, basicFragment);

	float triangle[] = {
        -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,
        0.5f,  -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,
         0.0f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f
    };

    Model& triangleMesh = addModel(triangle, std::size(triangle), GL_TRIANGLES);

	addObject(triangleMesh, basic);
}

void Scene::createSphere()
{
    clear();
    spinning = true;

	// Create and compile the vertex and fragment shaders
	Shader vertex(GL_VERTEX_SHADER, "shaders/transform3D.vert");
	Shader basicFragment(GL_FRAGMENT_SHADER, "shaders/basic.frag");

	// Create and link the shader program 
	ShaderProgram& basic = addShaderProgram(vertex, basicFragment);

	Model& sphereMesh = addModel(sphere, std::size(sphere), GL_TRIANGLES);

	addObject(sphereMesh, basic);
}

void Scene::createForest()
{
    clear();

    // Create and compile the vertex and fragment shaders
    Shader vertex(GL_VERTEX_SHADER, "shaders/transform3D.vert");
    Shader basicFragment(GL_FRAGMENT_SHADER, "shaders/basic.frag");
    Shader sunFragment(GL_FRAGMENT_SHADER, "shaders/sun.frag");

    // Create and link the shader program 
    ShaderProgram& basic = addShaderProgram(vertex, basicFragment);
    ShaderProgram& sunProgram = addShaderProgram(vertex, sunFragment);

    Model& treeMesh = addModel(tree, std::size(tree), GL_TRIANGLES);
    Model& bushMesh = addModel(bushes, std::size(bushes), GL_TRIANGLES);
    Model& sphereMesh = addModel(sphere, std::size(sphere), GL_TRIANGLES);

    // 6 trees in the back row, 5 trees in the front row.
    for (int i = 0; i < 6; i++)
    {
        DrawableObject& treeObject = addObject(treeMesh, basic);
        treeObject.setScale(0.065f);
        treeObject.setTranslation(-0.8f + i * 0.32f, -0.1f, 0.4f);
    }
    for (int i = 0; i < 5; i++)
    {
        DrawableObject& treeObject = addObject(treeMesh, basic);
        treeObject.setScale(0.08f);
        treeObject.setTranslation(-0.72f + i * 0.36f, -0.7f, -0.2f);
    }

    // 12 bushes along the bottom.
    for (int i = 0; i < 11; i++)
    {
        DrawableObject& bushObject = addObject(bushMesh, basic);
        bushObject.setScale(0.22f);
        bushObject.setTranslation(-0.85f + i * 0.165f, -0.88f, -0.6f);
    }

    DrawableObject& sun = addObject(sphereMesh, sunProgram);
    sun.setScale(0.13f);
    sun.setTranslation(0.72f, 0.75f, 0.65f);
}

void Scene::createLogin()
{
    clear();
    spinning = true;

    // Create and compile the vertex and fragment shaders
    Shader vertex(GL_VERTEX_SHADER, "shaders/transform3D.vert");
	Shader basicFragment(GL_FRAGMENT_SHADER, "shaders/basic.frag");

    // Create and link the shader program 
	ShaderProgram& basic = addShaderProgram(vertex, basicFragment);

    Model& loginMesh = addModel(cho0289, std::size(cho0289), GL_TRIANGLES);

	addObject(loginMesh, basic);
}

void Scene::draw()
{
    if (spinning)
    {
        rotationAngle += 0.01f;
    }
    for (const auto& object : objects)
    {
        if (spinning)
        {
            object->setRotationAngle(rotationAngle);
        }
        object->draw();
    }
}

void Scene::clear()
{
    objects.clear();
    models.clear();
    shaderPrograms.clear();

    spinning = false;
    rotationAngle = 0.0f;
}

Scene::~Scene()
{
	clear();
}
