#include "SceneFactory.h"

#include "Scene.h"
#include "Shader.h"
#include "transformations/TransformationFactory.h"
#include "transformations/CompositeTransformation.h"

#include <clock.h>
#include <sphere.h>
#include <bushes.h>
#include <tree.h>
#include <CHO0289.h>
#include <sun.h>
#include <earth.h>
#include <moon.h>
#include <plain.h>
#include <rocket.h>
#include <random>

#include <iterator>
#include <memory>
#include <utility>

std::unique_ptr<Scene> SceneFactory::createSignature()
{
    auto scene = std::make_unique<Scene>();

    // Create and compile the vertex and fragment shaders
    Shader vertex(GL_VERTEX_SHADER, "shaders/transform3D.vert");
    Shader fragment(GL_FRAGMENT_SHADER, "shaders/constant.frag");

    // Create and link the shader program
    ShaderProgram& program = scene->addShaderProgram(vertex, fragment, false);

    Model& loginMesh = scene->addModel(cho0289, std::size(cho0289), VertexFormat::PositionNormal);

    Material& signatureMaterial = scene->addMaterial();
    DrawableObject& loginObject = scene->addObject(loginMesh, program, signatureMaterial);

    auto translation = TransformationFactory::makeTranslation(0.83f, -0.95f, 0.0f);

    auto scale = TransformationFactory::makeScale(0.15f);

    auto transformation = std::make_unique<CompositeTransformation>();
    transformation->add(std::move(translation));
    transformation->add(std::move(scale));

    loginObject.setTransformation(std::move(transformation));

    ShaderProgram::unuse();
    return scene;
}

std::unique_ptr<Scene> SceneFactory::createClock()
{
    auto scene = std::make_unique<Scene>();

    // Create and compile the vertex and fragment shaders
    Shader vertex(GL_VERTEX_SHADER, "shaders/clock.vert");
    Shader basicFragment(GL_FRAGMENT_SHADER, "shaders/clock.frag");

    // Create and link the shader program
    ShaderProgram& basic = scene->addShaderProgram(vertex, basicFragment);

    Model& clockMesh = scene->addModel(clockModel, std::size(clockModel), VertexFormat::PositionNormalColor);

    Material& clockMaterial = scene->addMaterial();
    clockMaterial.setUseVertexColor(true);
    DrawableObject& clockObject = scene->addObject(clockMesh, basic, clockMaterial);

    auto scale = TransformationFactory::makeScale(1.0f, 1.0f, -1.0f);

    clockObject.setTransformation(std::move(scale));

    ShaderProgram::unuse();
    return scene;
}

std::unique_ptr<Scene> SceneFactory::createForest()
{
    auto scene = std::make_unique<Scene>();

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
    ShaderProgram& treeProgram = scene->addShaderProgram(treeVertex, forestFragment);
    ShaderProgram& bushProgram = scene->addShaderProgram(bushVertex, forestFragment);
    ShaderProgram& groundProgram = scene->addShaderProgram(groundVertex, forestFragment);
    ShaderProgram& clockProgram = scene->addShaderProgram(clockVertex, clockFragment);
    ShaderProgram& sunProgram = scene->addShaderProgram(basicVertex, sunFragment);

    Model& treeMesh = scene->addModel(tree, std::size(tree), VertexFormat::PositionNormal);
    Model& bushMesh = scene->addModel(bushes, std::size(bushes), VertexFormat::PositionNormal);
	Model& groundMesh = scene->addModel(plain, std::size(plain), VertexFormat::PositionNormal);
	Model& clockMesh = scene->addModel(clockModel, std::size(clockModel), VertexFormat::PositionNormalColor);
    Model& sphereMesh = scene->addModel(sphere, std::size(sphere), VertexFormat::PositionNormal);

    // These shaders currently supply their own colors.
    Material& forestMaterial = scene->addMaterial();
    forestMaterial.setUseVertexColor(true);
    Material& sunMaterial = scene->addMaterial();
    sunMaterial.setColor(glm::vec3(1.0f, 0.85f, 0.1f));

    DrawableObject& ground = scene->addObject(groundMesh, groundProgram, forestMaterial);

    auto groundTransformation = std::make_unique<CompositeTransformation>();
    groundTransformation->add(TransformationFactory::makeTranslation(0.0f, -0.9f, -5.0f));
    groundTransformation->add(TransformationFactory::makeScale(6.0f, 1.0f, 7.0f));

    ground.setTransformation(std::move(groundTransformation));

    std::mt19937 generator(42);
    std::uniform_real_distribution<float> jitter(-0.20f, 0.20f);
    std::uniform_real_distribution<float> randomAngleDegrees(0.0f, 360.0f);
    std::uniform_real_distribution<float> randomTreeScale(0.18f, 0.28f);
    std::uniform_real_distribution<float> randomBushScale(0.7f, 1.3f);
    std::uniform_real_distribution<float> randomBushX(-4.8f, 4.8f);
    std::uniform_real_distribution<float> randomBushZ(-10.0f, -0.8f);

    // 80 trees with random positions, rotations and scales.
    for (int i = 0; i < 80; i++)
    {
        DrawableObject& treeObject = scene->addObject(treeMesh, treeProgram, forestMaterial);

        const float x = -4.5f + (i % 10) + jitter(generator);
        const float z = -1.0f - (i / 10) * 1.25f + jitter(generator);

        auto transformation = std::make_unique<CompositeTransformation>();

        transformation->add(TransformationFactory::makeTranslation(x, -0.9f, z));
        transformation->add(TransformationFactory::makeRotation(randomAngleDegrees(generator), 0.0f, 1.0f, 0.0f));
        transformation->add(TransformationFactory::makeScale(randomTreeScale(generator)));

        treeObject.setTransformation(std::move(transformation));
    }

    // 24 bushes spread across the ground.
    for (int i = 0; i < 24; i++)
    {
        DrawableObject& bushObject = scene->addObject(bushMesh, bushProgram, forestMaterial);

        const float x = randomBushX(generator);
        const float z = randomBushZ(generator);
        const float scale = randomBushScale(generator);

        auto transformation = std::make_unique<CompositeTransformation>();

        transformation->add(TransformationFactory::makeTranslation(x, -0.9f, z));
        transformation->add(TransformationFactory::makeRotation(randomAngleDegrees(generator), 0.0f, 1.0f, 0.0f));
        transformation->add(TransformationFactory::makeScale(scale));

        bushObject.setTransformation(std::move(transformation));
    }

    // 2 clocks
    for (int i = 0; i < 2; i++)
    {
        DrawableObject& clockObject = scene->addObject(clockMesh, clockProgram, forestMaterial);

        const float x = (i == 0) ? 0.0f : 2.0f;
        const float z = (i == 0) ? -3.0f : -7.9f;
        const float angleDegrees = (i == 0) ? 10.0f : -20.0f;
        const float scale = (i == 0) ? 0.18f : 0.22f;

        auto transformation = std::make_unique<CompositeTransformation>();

        transformation->add(TransformationFactory::makeTranslation(x, -0.9f + scale, z));
        transformation->add(TransformationFactory::makeRotation(angleDegrees, 0.0f, 1.0f, 0.0f));
        transformation->add(TransformationFactory::makeScale(scale, scale, -scale));

        clockObject.setTransformation(std::move(transformation));
    }

    DrawableObject& sun = scene->addObject(sphereMesh, sunProgram, sunMaterial);

    // Place a larger sun above and beyond the forest.
    auto translation = TransformationFactory::makeTranslation(8.0f, 9.0f, -30.0f);
    auto scale = TransformationFactory::makeScale(3.5f);

    auto transformation = std::make_unique<CompositeTransformation>();
    transformation->add(std::move(translation));
    transformation->add(std::move(scale));

    sun.setTransformation(std::move(transformation));
    ShaderProgram::unuse();
    return scene;
}

std::unique_ptr<Scene> SceneFactory::createSolarSystem()
{
    auto scene = std::make_unique<Scene>();

	Shader vertex(GL_VERTEX_SHADER, "shaders/transform3D.vert");
	Shader basicFragment(GL_FRAGMENT_SHADER, "shaders/basic.frag");

	ShaderProgram& basic = scene->addShaderProgram(vertex, basicFragment);

	Model& sunMesh = scene->addModel(sun, std::size(sun), VertexFormat::PositionColorNormal);
    Model& moonMesh = scene->addModel(moon, std::size(moon), VertexFormat::PositionColorNormal);
    Model& earthMesh = scene->addModel(earth, std::size(earth), VertexFormat::PositionColorNormal);

    Material& planetMaterial = scene->addMaterial();
    planetMaterial.setUseVertexColor(true);

	DrawableObject& sunObject = scene->addObject(sunMesh, basic, planetMaterial);
	DrawableObject& moonObject = scene->addObject(moonMesh, basic, planetMaterial);
	DrawableObject& earthObject = scene->addObject(earthMesh, basic, planetMaterial);

    // Sun rotates around its own axis.
    auto sunRotation = TransformationFactory::makeDynamicRotation(30.0f, 0.0f, 1.0f, 0.0f);
    auto sunScale = TransformationFactory::makeScale(0.2f);

    auto sunTransformation = std::make_unique<CompositeTransformation>();
    sunTransformation->add(std::move(sunRotation));
    sunTransformation->add(std::move(sunScale));

    sunObject.setTransformation(std::move(sunTransformation));

    const float earthOrbitDegreesPerSecond = 20.0f;
    const float earthOrbitRadius = 0.55f;

    // Earth orbits the Sun in the XY plane and rotates around its own axis.
    auto earthOrbit = TransformationFactory::makeDynamicRotation(earthOrbitDegreesPerSecond, 0.0f, 1.0f, 0.0f);
    auto earthTranslation = TransformationFactory::makeTranslation(earthOrbitRadius, 0.0f, 0.0f);
    auto earthRotation = TransformationFactory::makeDynamicRotation(70.0f, 0.0f, 1.0f, 0.0f);
    auto earthScale = TransformationFactory::makeScale(0.1f);

    auto earthTransformation = std::make_unique<CompositeTransformation>();
    earthTransformation->add(std::move(earthOrbit));
    earthTransformation->add(std::move(earthTranslation));
    earthTransformation->add(std::move(earthRotation));
    earthTransformation->add(std::move(earthScale));

    earthObject.setTransformation(std::move(earthTransformation));

    // Moon follows Earth's orbit, then adds its own orbit around Earth.
    auto moonEarthOrbit = TransformationFactory::makeDynamicRotation(earthOrbitDegreesPerSecond, 0.0f, 1.0f, 0.0f);
    auto moonEarthTranslation = TransformationFactory::makeTranslation(earthOrbitRadius, 0.0f, 0.0f);
    auto moonOrbit = TransformationFactory::makeDynamicRotation(-80.0f, 0.0f, 1.0f, 0.0f);
    auto moonTranslation = TransformationFactory::makeTranslation(0.18f, 0.0f, 0.0f);
    auto moonScale = TransformationFactory::makeScale(0.035f);

    auto moonTransformation = std::make_unique<CompositeTransformation>();
    moonTransformation->add(std::move(moonEarthOrbit));
    moonTransformation->add(std::move(moonEarthTranslation));
    moonTransformation->add(std::move(moonOrbit));
    moonTransformation->add(std::move(moonTranslation));
    moonTransformation->add(std::move(moonScale));

    moonObject.setTransformation(std::move(moonTransformation));
    ShaderProgram::unuse();
    return scene;
}

std::unique_ptr<Scene> SceneFactory::createLightingTest()
{
    auto scene = std::make_unique<Scene>();

    scene->configureCamera(179.0f, 270.0f, glm::vec3(0.0f, 10.0f, 0.175f), 45.0f);

    Shader vertex(GL_VERTEX_SHADER, "shaders/lighting.vert");
    Shader constantFragment(GL_FRAGMENT_SHADER, "shaders/constant.frag");
    Shader lambertFragment(GL_FRAGMENT_SHADER, "shaders/lambert.frag");
    Shader phongFragment(GL_FRAGMENT_SHADER, "shaders/phong.frag");
    Shader blinnFragment(GL_FRAGMENT_SHADER, "shaders/blinn.frag");

    ShaderProgram& constantProgram = scene->addShaderProgram(vertex, constantFragment);
    ShaderProgram& lambertProgram = scene->addShaderProgram(vertex, lambertFragment, true, true);
    ShaderProgram& phongProgram = scene->addShaderProgram(vertex, phongFragment, true, true);
    ShaderProgram& blinnProgram = scene->addShaderProgram(vertex, blinnFragment, true, true);

    const glm::vec3 sphereColor(0.385f, 0.647f, 0.812f);

    Material& constantMaterial = scene->addMaterial();
    constantMaterial.setColor(sphereColor);

    Material& lambertMaterial = scene->addMaterial();
    lambertMaterial.setColor(sphereColor);
    lambertMaterial.setAmbientStrength(0.0f);

    Material& phongMaterial = scene->addMaterial();
    phongMaterial.setColor(sphereColor);
    phongMaterial.setAmbientStrength(0.08f);
    phongMaterial.setSpecularStrength(0.8f);
    phongMaterial.setShininess(32.0f);

    Material& blinnMaterial = scene->addMaterial();
    blinnMaterial.setColor(sphereColor);
    blinnMaterial.setAmbientStrength(0.08f);
    blinnMaterial.setSpecularStrength(0.8f);
    blinnMaterial.setShininess(128.0f);

    ShaderProgram::unuse();

	Model& sphereMesh = scene->addModel(sphere, std::size(sphere), VertexFormat::PositionNormal);

    const glm::vec3 positions[] = {
        { 2.0f, 0.0f, 0.0f},
        { 0.0f, 0.0f, -2.0f},
        { 0.0f, 0.0f,  2.0f},
        { -2.0f, 0.0f,  0.0f}
    };

    ShaderProgram* programs[] = {
        &constantProgram, &lambertProgram, &phongProgram, &blinnProgram
    };

    Material* sphereMaterials[] = {
        &constantMaterial, &lambertMaterial, &phongMaterial, &blinnMaterial
    };

    for (std::size_t i = 0; i < std::size(positions); i++)
    {
        DrawableObject& object = scene->addObject(sphereMesh, *programs[i], *sphereMaterials[i]);

        const glm::vec3& position = positions[i];

        auto transformation = std::make_unique<CompositeTransformation>();

        transformation->add(TransformationFactory::makeTranslation(position.x, position.y, position.z));
        transformation->add(TransformationFactory::makeScale(0.85f));

        object.setTransformation(std::move(transformation));
    }
    ShaderProgram::unuse();
    return scene;
}

std::unique_ptr<Scene> SceneFactory::createLitSolarSystem()
{
    auto scene = std::make_unique<Scene>();

    scene->configureCamera(125.0f, 270.0f, glm::vec3(0.0f, 9.0f, 13.0f), 60.0f);

	Shader lightingVertex(GL_VERTEX_SHADER, "shaders/lighting.vert");
	Shader constantFragment(GL_FRAGMENT_SHADER, "shaders/constant.frag");
	Shader phongFragment(GL_FRAGMENT_SHADER, "shaders/phong.frag");
	Shader blinnFragment(GL_FRAGMENT_SHADER, "shaders/blinn.frag");

	ShaderProgram& constantProgram = scene->addShaderProgram(lightingVertex, constantFragment);
	ShaderProgram& phongProgram = scene->addShaderProgram(lightingVertex, phongFragment, true, true);
	ShaderProgram& blinnProgram = scene->addShaderProgram(lightingVertex, blinnFragment, true, true);

    Material& sunMaterial = scene->addMaterial();
    sunMaterial.setUseVertexColor(true);
    sunMaterial.setColor(glm::vec3(1.0f, 0.75f, 0.1f));

    Material& earthMaterial = scene->addMaterial();
    earthMaterial.setUseVertexColor(true);
    earthMaterial.setColor(glm::vec3(0.15f, 0.4f, 0.85f));
    earthMaterial.setShininess(32.0f);

    Material& moonMaterial = scene->addMaterial();
    moonMaterial.setUseVertexColor(true);
    moonMaterial.setColor(glm::vec3(0.65f, 0.65f, 0.68f));
    moonMaterial.setShininess(128.0f);
    moonMaterial.setSpecularStrength(0.25f);

    ShaderProgram::unuse();

    Model& sunMesh = scene->addModel(sun, std::size(sun), VertexFormat::PositionColorNormal);
    Model& earthMesh = scene->addModel(earth, std::size(earth), VertexFormat::PositionColorNormal);
    Model& moonMesh = scene->addModel(moon, std::size(moon), VertexFormat::PositionColorNormal);
	Model& rocketMesh = scene->addModel(rocket, std::size(rocket), VertexFormat::PositionColorNormal);

    DrawableObject& sunObject = scene->addObject(sunMesh, constantProgram, sunMaterial);
    DrawableObject& earthObject = scene->addObject(earthMesh, phongProgram, earthMaterial);
    DrawableObject& moonObject = scene->addObject(moonMesh, blinnProgram, moonMaterial);
	DrawableObject& rocketObject = scene->addObject(rocketMesh, blinnProgram, moonMaterial);

    auto sunTransformation = std::make_unique<CompositeTransformation>();

    sunTransformation->add(TransformationFactory::makeScale(1.2f));
	sunTransformation->add(TransformationFactory::makeDynamicRotation(30.0f, 0.0f, 1.0f, 0.0f));

	sunObject.setTransformation(std::move(sunTransformation));

    const float earthOrbitDegreesPerSecond = 20.0f;
    const float earthOrbitRadius = 4.5f;

    auto earthTransformation = std::make_unique<CompositeTransformation>();

    // Orbit around the Sun.
    earthTransformation->add(TransformationFactory::makeDynamicRotation(earthOrbitDegreesPerSecond, 0.0f, 1.0f, 0.0f));
    earthTransformation->add(TransformationFactory::makeTranslation(earthOrbitRadius, 0.0f, 0.0f));

    // Rotate around Earth's own axis.
    earthTransformation->add(TransformationFactory::makeDynamicRotation(70.0f, 0.0f, 1.0f, 0.0f));
    earthTransformation->add(TransformationFactory::makeScale(0.65f));

    earthObject.setTransformation(std::move(earthTransformation));

    auto moonTransformation = std::make_unique<CompositeTransformation>();

    // Follow Earth's orbital position.
    moonTransformation->add(TransformationFactory::makeDynamicRotation(earthOrbitDegreesPerSecond, 0.0f, 1.0f, 0.0f));
    moonTransformation->add(TransformationFactory::makeTranslation(earthOrbitRadius, 0.0f, 0.0f));

    // Orbit around Earth in its orbital frame.
    moonTransformation->add(TransformationFactory::makeDynamicRotation(80.0f, 0.0f, 1.0f, 0.0f));
    moonTransformation->add(TransformationFactory::makeTranslation(1.3f, 0.0f, 0.0f));
    moonTransformation->add(TransformationFactory::makeScale(0.25f));

    moonObject.setTransformation(std::move(moonTransformation));

	auto rocketTransformation = std::make_unique<CompositeTransformation>();

	rocketTransformation->add(TransformationFactory::makeDynamicRotation(earthOrbitDegreesPerSecond, 0.0f, 1.0f, 0.0f));
	rocketTransformation->add(TransformationFactory::makeTranslation(earthOrbitRadius, 0.0f, 0.0f));

    rocketTransformation->add(TransformationFactory::makeDynamicRotation(80.0f, 0.0f, 0.0f, 1.0f, 180.0f));
    rocketTransformation->add(TransformationFactory::makeTranslation(1.3f, 0.0f, 0.0f));
	rocketTransformation->add(TransformationFactory::makeScale(0.25f));

	rocketObject.setTransformation(std::move(rocketTransformation));
    ShaderProgram::unuse();
    return scene;
}


std::unique_ptr<Scene> SceneFactory::createNightForest()
{
    auto scene = std::make_unique<Scene>();

    scene->configureCamera(100.0f, 270.0f, glm::vec3(0.0f, 1.5f, 3.0f), 60.0f);

    const glm::vec3 fireflyPosition(0.0f, 0.6f, -3.0f);
    const glm::vec3 fireflyColor(1.0f, 0.95f, 0.55f);
    scene->configureLight(fireflyPosition, fireflyColor, 1.0f, 0.7f, 1.8f);

    Shader vertex(GL_VERTEX_SHADER, "shaders/lighting.vert");
    Shader phongFragment(GL_FRAGMENT_SHADER, "shaders/phong.frag");
    Shader constantFragment(GL_FRAGMENT_SHADER, "shaders/constant.frag");

    ShaderProgram& forestProgram = scene->addShaderProgram(vertex, phongFragment, true, true);
    ShaderProgram& fireflyProgram = scene->addShaderProgram(vertex, constantFragment);

    Material& treeMaterial = scene->addMaterial();
    treeMaterial.setColor(glm::vec3(0.18f, 0.40f, 0.12f));
    treeMaterial.setAmbientStrength(0.015f);
    treeMaterial.setSpecularStrength(0.1f);
    treeMaterial.setShininess(16.0f);

    Material& bushMaterial = scene->addMaterial();
    bushMaterial.setColor(glm::vec3(0.12f, 0.32f, 0.10f));
    bushMaterial.setAmbientStrength(0.015f);
    bushMaterial.setSpecularStrength(0.08f);
    bushMaterial.setShininess(8.0f);

    Material& groundMaterial = scene->addMaterial();
    groundMaterial.setColor(glm::vec3(0.25f, 0.20f, 0.12f));
    groundMaterial.setAmbientStrength(0.015f);
    groundMaterial.setSpecularStrength(0.05f);
    groundMaterial.setShininess(8.0f);

    Material& fireflyMaterial = scene->addMaterial();
    fireflyMaterial.setColor(fireflyColor);

    Model& treeMesh = scene->addModel(tree, std::size(tree), VertexFormat::PositionNormal);
    Model& bushMesh = scene->addModel(bushes, std::size(bushes), VertexFormat::PositionNormal);
    Model& groundMesh = scene->addModel(plain, std::size(plain), VertexFormat::PositionNormal);
    Model& fireflyMesh = scene->addModel(sphere, std::size(sphere), VertexFormat::PositionNormal);

    DrawableObject& ground = scene->addObject(groundMesh, forestProgram, groundMaterial);
    auto groundTransform = std::make_unique<CompositeTransformation>();
    groundTransform->add(TransformationFactory::makeTranslation(0.0f, -0.9f, -5.0f));
    groundTransform->add(TransformationFactory::makeScale(6.0f, 1.0f, 7.0f));
    ground.setTransformation(std::move(groundTransform));

    std::mt19937 generator(42);
    std::uniform_real_distribution<float> jitter(-0.20f, 0.20f);
    std::uniform_real_distribution<float> randomAngleDegrees(0.0f, 360.0f);
    std::uniform_real_distribution<float> randomTreeScale(0.18f, 0.28f);
    std::uniform_real_distribution<float> randomBushScale(0.7f, 1.3f);
    std::uniform_real_distribution<float> randomBushX(-4.8f, 4.8f);
    std::uniform_real_distribution<float> randomBushZ(-10.0f, -0.8f);

    // 80 trees with random positions, rotations and scales.
    for (int i = 0; i < 80; i++)
    {
        DrawableObject& treeObject = scene->addObject(treeMesh, forestProgram, treeMaterial);

        const float x = -4.5f + (i % 10) + jitter(generator);
        const float z = -1.0f - (i / 10) * 1.25f + jitter(generator);

        auto transformation = std::make_unique<CompositeTransformation>();

        transformation->add(TransformationFactory::makeTranslation(x, -0.9f, z));
        transformation->add(TransformationFactory::makeRotation(randomAngleDegrees(generator), 0.0f, 1.0f, 0.0f));
        transformation->add(TransformationFactory::makeScale(randomTreeScale(generator)));

        treeObject.setTransformation(std::move(transformation));
    }

    // 24 bushes spread across the ground.
    for (int i = 0; i < 24; i++)
    {
        DrawableObject& bushObject = scene->addObject(bushMesh, forestProgram, bushMaterial);

        const float x = randomBushX(generator);
        const float z = randomBushZ(generator);
        const float scale = randomBushScale(generator);

        auto transformation = std::make_unique<CompositeTransformation>();

        transformation->add(TransformationFactory::makeTranslation(x, -0.9f, z));
        transformation->add(TransformationFactory::makeRotation(randomAngleDegrees(generator), 0.0f, 1.0f, 0.0f));
        transformation->add(TransformationFactory::makeScale(scale));

        bushObject.setTransformation(std::move(transformation));
    }

    // The visible firefly and the point light share the same world position.
    DrawableObject& firefly = scene->addObject(fireflyMesh, fireflyProgram, fireflyMaterial);
    auto fireflyTransform = std::make_unique<CompositeTransformation>();
    fireflyTransform->add(TransformationFactory::makeTranslation(fireflyPosition.x, fireflyPosition.y, fireflyPosition.z));
    fireflyTransform->add(TransformationFactory::makeScale(0.06f));
    firefly.setTransformation(std::move(fireflyTransform));

    ShaderProgram::unuse();
    return scene;
}
