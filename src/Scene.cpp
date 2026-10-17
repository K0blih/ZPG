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
#include <clock.h>
#include <plain.h>

void Scene::rotateCamera(float deltaX, float deltaY)
{
    camera.rotate(deltaX, deltaY);
}

void Scene::resizeCamera(int width, int height)
{
	camera.resize(width, height);
}

void Scene::moveCamera(float forwardInput, float rightInput, float deltaTime)
{
	camera.move(forwardInput, rightInput, deltaTime);
}

Model& Scene::addModel(const float* vertices, std::size_t floatCount, VertexFormat format, GLenum drawingMode)
{
    auto model = std::make_unique<Model>();
    model->create(vertices, floatCount, format, drawingMode);

    models.push_back(std::move(model));
    return *models.back();
}

ShaderProgram& Scene::addShaderProgram(const Shader& vertexShader, const Shader& fragmentShader, bool useCamera)
{
    auto program = std::make_unique<ShaderProgram>();
    program->link(vertexShader, fragmentShader);

    shaderPrograms.push_back(std::move(program));

    ShaderProgram& result = *shaderPrograms.back();

	if (useCamera)
	{
        camera.subscribe(result);
	}

    return result;
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
    ShaderProgram& program = addShaderProgram(vertex, fragment, false);

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
    auto rotation = TransformationFactory::makeRotation(1.0f, 0.0f, 1.0f, 0.0f);

    auto transformation = std::make_unique<CompositeTransformation>();
    transformation->add(std::move(translation));
    transformation->add(std::move(rotation));

    triangleObject.setTransformation(std::move(transformation));
}

void Scene::createClock()
{
    clear();

    // Create and compile the vertex and fragment shaders
    Shader vertex(GL_VERTEX_SHADER, "shaders/clock.vert");
    Shader basicFragment(GL_FRAGMENT_SHADER, "shaders/clock.frag");

    // Create and link the shader program 
    ShaderProgram& basic = addShaderProgram(vertex, basicFragment);

    Model& clockMesh = addModel(clockModel, std::size(clockModel), VertexFormat::PositionNormalColor);

    DrawableObject& clockObject = addObject(clockMesh, basic);

    auto scale = TransformationFactory::makeScale(1.0f, 1.0f, -1.0f);

    clockObject.setTransformation(std::move(scale));
}

void Scene::createForest()
{
    clear();

    // Create and compile the vertex and fragment shaders
    Shader treeVertex(GL_VERTEX_SHADER, "shaders/forest.vert");
    Shader bushVertex(GL_VERTEX_SHADER, "shaders/bush.vert");
    Shader groundVertex(GL_VERTEX_SHADER, "shaders/ground.vert");
    Shader forestFragment(GL_FRAGMENT_SHADER, "shaders/forest.frag");

    Shader clockVertex(GL_VERTEX_SHADER, "shaders/clock.vert");
    Shader clockFragment(GL_FRAGMENT_SHADER, "shaders/clock.frag");

    Shader basicVertex(GL_VERTEX_SHADER, "shaders/transform3D.vert");
    Shader sunFragment(GL_FRAGMENT_SHADER, "shaders/sun.frag");

    // Create and link the shader program 
    ShaderProgram& treeProgram = addShaderProgram(treeVertex, forestFragment);
    ShaderProgram& bushProgram = addShaderProgram(bushVertex, forestFragment);
    ShaderProgram& groundProgram = addShaderProgram(groundVertex, forestFragment);
    ShaderProgram& clockProgram = addShaderProgram(clockVertex, clockFragment);
    ShaderProgram& sunProgram = addShaderProgram(basicVertex, sunFragment);

    Model& treeMesh = addModel(tree, std::size(tree), VertexFormat::PositionNormal);
    Model& bushMesh = addModel(bushes, std::size(bushes), VertexFormat::PositionNormal);
	Model& groundMesh = addModel(plain, std::size(plain), VertexFormat::PositionNormal);
	Model& clockMesh = addModel(clockModel, std::size(clockModel), VertexFormat::PositionNormalColor);
    Model& sphereMesh = addModel(sphere, std::size(sphere), VertexFormat::PositionNormal);

    DrawableObject& ground = addObject(groundMesh, groundProgram);

    auto groundTransformation = std::make_unique<CompositeTransformation>();
    groundTransformation->add(TransformationFactory::makeTranslation(0.0f, -0.9f, -5.0f));
    groundTransformation->add(TransformationFactory::makeScale(6.0f, 1.0f, 7.0f));

    ground.setTransformation(std::move(groundTransformation));

    std::mt19937 generator(std::random_device{}());
    std::uniform_real_distribution<float> jitter(-0.20f, 0.20f);
    std::uniform_real_distribution<float> randomAngle(0.0f, 6.2831853f);
    std::uniform_real_distribution<float> randomTreeScale(0.18f, 0.28f);
    std::uniform_real_distribution<float> randomBushScale(0.7f, 1.3f);
    std::uniform_real_distribution<float> randomBushX(-4.8f, 4.8f);
    std::uniform_real_distribution<float> randomBushZ(-10.0f, -0.8f);

    // 80 trees with random positions, rotations and scales.
    for (int i = 0; i < 80; i++)
    {
        DrawableObject& treeObject = addObject(treeMesh, treeProgram);

        const float x = -4.5f + (i % 10) + jitter(generator);
        const float z = -1.0f - (i / 10) * 1.25f + jitter(generator);

        auto transformation = std::make_unique<CompositeTransformation>();

        transformation->add(TransformationFactory::makeTranslation(x, -0.9f, z));
        transformation->add(TransformationFactory::makeRotation(randomAngle(generator), 0.0f, 1.0f, 0.0f));
        transformation->add(TransformationFactory::makeScale(randomTreeScale(generator)));

        treeObject.setTransformation(std::move(transformation));
    }

    // 24 bushes spread across the ground.
    for (int i = 0; i < 24; i++)
    {
        DrawableObject& bushObject = addObject(bushMesh, bushProgram);

        const float x = randomBushX(generator);
        const float z = randomBushZ(generator);
        const float scale = randomBushScale(generator);

        auto transformation = std::make_unique<CompositeTransformation>();

        transformation->add(TransformationFactory::makeTranslation(x, -0.9f, z));
        transformation->add(TransformationFactory::makeRotation(randomAngle(generator), 0.0f, 1.0f, 0.0f));
        transformation->add(TransformationFactory::makeScale(scale));

        bushObject.setTransformation(std::move(transformation));
    }

    // 2 clocks
    for (int i = 0; i < 2; i++)
    {
        DrawableObject& clockObject = addObject(clockMesh, clockProgram);

        const float x = (i == 0) ? 0.0f : 2.0f;
        const float z = (i == 0) ? -3.0f : -7.9f;
        const float angle = (i == 0) ? 0.20f : -0.35f;
        const float scale = (i == 0) ? 0.18f : 0.22f;

        auto transformation = std::make_unique<CompositeTransformation>();

        transformation->add(TransformationFactory::makeTranslation(x, -0.9f + scale, z));
        transformation->add(TransformationFactory::makeRotation(angle, 0.0f, 1.0f, 0.0f));
        transformation->add(TransformationFactory::makeScale(scale, scale, -scale));

        clockObject.setTransformation(std::move(transformation));
    }

    DrawableObject& sun = addObject(sphereMesh, sunProgram);

    // Place a larger sun above and beyond the forest.
    auto translation = TransformationFactory::makeTranslation(8.0f, 9.0f, -30.0f);
    auto scale = TransformationFactory::makeScale(3.5f);

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
    auto earthOrbit = TransformationFactory::makeDynamicRotation(earthOrbitSpeed, 0.0f, 1.0f, 0.0f);
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
    auto moonEarthOrbit = TransformationFactory::makeDynamicRotation(earthOrbitSpeed, 0.0f, 1.0f, 0.0f);
    auto moonEarthTranslation = TransformationFactory::makeTranslation(earthOrbitRadius, 0.0f, 0.0f);
    auto moonOrbit = TransformationFactory::makeDynamicRotation(-1.4f, 0.0f, 1.0f, 0.0f);
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
    for (const auto& program : shaderPrograms) {
        camera.unsubscribe(*program);
    }

    objects.clear();
    models.clear();
    shaderPrograms.clear();
}

Scene::~Scene()
{
	clear();
}
