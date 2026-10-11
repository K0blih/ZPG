#include "Scene.h"
#include "Shader.h"
#include "transformations/TransformationFactory.h"
#include "transformations/CompositeTransformation.h"
#include "VertexFormat.h"

#include <iterator>
#include <memory>
#include <utility>
#include <random>

#include <sphere.h>
#include <bushes.h>
#include <tree.h>
#include <CHO0289.h>
#include <sun.h>
#include <earth.h>
#include <moon.h>

Model& Scene::addModel(const float* vertices, std::size_t floatCount, VertexFormat format, GLenum drawingMode)
{
    auto model = std::make_unique<Model>();
    model->create(vertices, floatCount, format, drawingMode);

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

    Model& loginMesh = addModel(cho0289, std::size(cho0289), VertexFormat::PositionNormal);

    DrawableObject& loginObject = addObject(loginMesh, program);

	auto translation = TransformationFactory::makeTranslation(0.83f, -0.95f, 0.0f);

	auto scale = TransformationFactory::makeScale(0.15f);

	auto transformation = std::make_unique<CompositeTransformation>();
	transformation->add(std::move(translation));
	transformation->add(std::move(scale));

    loginObject.setTransformation(std::move(transformation));
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

    Model& triangleMesh = addModel(triangle, std::size(triangle), VertexFormat::PositionColor);

	DrawableObject& triangleObject = addObject(triangleMesh, basic);

    auto translation = TransformationFactory::makeTranslation(0.5f, 0.0f, 0.0f);
    auto scale = TransformationFactory::makeScale(0.5f);

    auto transformation = std::make_unique<CompositeTransformation>();
    transformation->add(std::move(translation));
    transformation->add(std::move(scale));

    triangleObject.setTransformation(std::move(transformation));
}

void Scene::createSphere()
{
    clear();

	// Create and compile the vertex and fragment shaders
	Shader vertex(GL_VERTEX_SHADER, "shaders/transform3D.vert");
	Shader basicFragment(GL_FRAGMENT_SHADER, "shaders/basic.frag");

	// Create and link the shader program 
	ShaderProgram& basic = addShaderProgram(vertex, basicFragment);

	Model& sphereMesh = addModel(sphere, std::size(sphere), VertexFormat::PositionNormal);

	DrawableObject& sphereObject = addObject(sphereMesh, basic);

	auto rotation = TransformationFactory::makeDynamicRotation(0.5f, 0.0f, 1.0f, 0.0f);

    sphereObject.setTransformation(std::move(rotation));
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

    Model& treeMesh = addModel(tree, std::size(tree), VertexFormat::PositionNormal);
    Model& bushMesh = addModel(bushes, std::size(bushes), VertexFormat::PositionNormal);
    Model& sphereMesh = addModel(sphere, std::size(sphere), VertexFormat::PositionNormal);

    std::mt19937 generator(std::random_device{}());
    std::uniform_real_distribution<float> randomX(-0.7f, 0.7f);
    std::uniform_real_distribution<float> randomTreeY(-0.8f, -0.2f);
    std::uniform_real_distribution<float> randomBushY(-0.88f, -0.55f);
    std::uniform_real_distribution<float> randomAngle(0.0f, 6.0f);
    std::uniform_real_distribution<float> randomTreeScale(0.05f, 0.08f);
    std::uniform_real_distribution<float> randomBushScale(0.25f, 0.45f);

    // 11 trees with random positions, rotations and scales.
    for (int i = 0; i < 11; i++)
    {
        DrawableObject& treeObject = addObject(treeMesh, basic);

        float x = randomX(generator);
        float y = randomTreeY(generator);

        auto translation = TransformationFactory::makeTranslation(x, y, y + 0.5f);
        auto rotation = TransformationFactory::makeRotation(randomAngle(generator), 0.0f, 1.0f, 0.0f);
        auto scale = TransformationFactory::makeScale(randomTreeScale(generator));

        auto transformation = std::make_unique<CompositeTransformation>();
        transformation->add(std::move(translation));
        transformation->add(std::move(rotation));
        transformation->add(std::move(scale));

        treeObject.setTransformation(std::move(transformation));
    }

    // 11 bushes with random positions, rotations and scales.
    for (int i = 0; i < 11; i++)
    {
        DrawableObject& bushObject = addObject(bushMesh, basic);

        float x = randomX(generator);
        float y = randomBushY(generator);

        auto translation = TransformationFactory::makeTranslation(x, y, y + 0.5f);
        auto rotation = TransformationFactory::makeRotation(randomAngle(generator), 0.0f, 1.0f, 0.0f);
        auto scale = TransformationFactory::makeScale(randomBushScale(generator));

        auto transformation = std::make_unique<CompositeTransformation>();
        transformation->add(std::move(translation));
        transformation->add(std::move(rotation));
        transformation->add(std::move(scale));

        bushObject.setTransformation(std::move(transformation));
    }

    DrawableObject& sun = addObject(sphereMesh, sunProgram);

    auto translation = TransformationFactory::makeTranslation(0.72f, 0.75f, 0.65f);
    auto scale = TransformationFactory::makeScale(0.13f);

    auto transformation = std::make_unique<CompositeTransformation>();
    transformation->add(std::move(translation));
    transformation->add(std::move(scale));

    sun.setTransformation(std::move(transformation));
}

void Scene::createLogin()
{
    clear();

    // Create and compile the vertex and fragment shaders
    Shader vertex(GL_VERTEX_SHADER, "shaders/transform3D.vert");
	Shader basicFragment(GL_FRAGMENT_SHADER, "shaders/basic.frag");

    // Create and link the shader program 
	ShaderProgram& basic = addShaderProgram(vertex, basicFragment);

    Model& loginMesh = addModel(cho0289, std::size(cho0289), VertexFormat::PositionNormal);

	DrawableObject& login = addObject(loginMesh, basic);

	auto rotation = TransformationFactory::makeDynamicRotation(0.5f, 0.0f, 1.0f, 0.0f);

	login.setTransformation(std::move(rotation));
}

void Scene::createSolarSystem()
{
    clear();

	Shader vertex(GL_VERTEX_SHADER, "shaders/transform3D.vert");
	Shader basicFragment(GL_FRAGMENT_SHADER, "shaders/basic.frag");

	ShaderProgram& basic = addShaderProgram(vertex, basicFragment);

	Model& sunMesh = addModel(sun, std::size(sun), VertexFormat::PositionColorNormal);
    Model& moonMesh = addModel(moon, std::size(moon), VertexFormat::PositionColorNormal);
    Model& earthMesh = addModel(earth, std::size(earth), VertexFormat::PositionColorNormal);

	DrawableObject& sunObject = addObject(sunMesh, basic);
	DrawableObject& moonObject = addObject(moonMesh, basic);
	DrawableObject& earthObject = addObject(earthMesh, basic);

    // Sun rotates around its own axis.
    auto sunRotation = TransformationFactory::makeDynamicRotation(0.5f, 0.0f, 1.0f, 0.0f);
    auto sunScale = TransformationFactory::makeScale(0.2f);

    auto sunTransformation = std::make_unique<CompositeTransformation>();
    sunTransformation->add(std::move(sunRotation));
    sunTransformation->add(std::move(sunScale));

    sunObject.setTransformation(std::move(sunTransformation));

    const float earthOrbitSpeed = 0.35f;
    const float earthOrbitRadius = 0.55f;

    // Earth orbits the Sun in the XY plane and rotates around its own axis.
    auto earthOrbit = TransformationFactory::makeDynamicRotation(earthOrbitSpeed, 0.0f, 0.0f, 1.0f);
    auto earthTranslation = TransformationFactory::makeTranslation(earthOrbitRadius, 0.0f, 0.0f);
    auto earthRotation = TransformationFactory::makeDynamicRotation(1.2f, 0.0f, 1.0f, 0.0f);
    auto earthScale = TransformationFactory::makeScale(0.1f);

    auto earthTransformation = std::make_unique<CompositeTransformation>();
    earthTransformation->add(std::move(earthOrbit));
    earthTransformation->add(std::move(earthTranslation));
    earthTransformation->add(std::move(earthRotation));
    earthTransformation->add(std::move(earthScale));

    earthObject.setTransformation(std::move(earthTransformation));

    // Moon follows Earth's orbit, then adds its own orbit around Earth.
    auto moonEarthOrbit = TransformationFactory::makeDynamicRotation(earthOrbitSpeed, 0.0f, 0.0f, 1.0f);
    auto moonEarthTranslation = TransformationFactory::makeTranslation(earthOrbitRadius, 0.0f, 0.0f);
    auto moonOrbit = TransformationFactory::makeDynamicRotation(1.4f, 0.0f, 0.0f, 1.0f);
    auto moonTranslation = TransformationFactory::makeTranslation(0.18f, 0.0f, 0.0f);
    auto moonScale = TransformationFactory::makeScale(0.035f);

    auto moonTransformation = std::make_unique<CompositeTransformation>();
    moonTransformation->add(std::move(moonEarthOrbit));
    moonTransformation->add(std::move(moonEarthTranslation));
    moonTransformation->add(std::move(moonOrbit));
    moonTransformation->add(std::move(moonTranslation));
    moonTransformation->add(std::move(moonScale));

    moonObject.setTransformation(std::move(moonTransformation));
}

void Scene::draw(double timeSeconds)
{
    for (const auto& object : objects)
    {
        object->draw(timeSeconds);
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
