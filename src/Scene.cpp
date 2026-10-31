#include "Scene.h"
#include "Shader.h"

#include <memory>
#include <utility>

void Scene::configureCamera(float alphaDegrees, float fiDegrees, const glm::vec3& position, float fieldOfViewDegrees)
{
	camera.setPosition(position);
	camera.setAngles(alphaDegrees, fiDegrees);
	camera.setFieldOfView(fieldOfViewDegrees);
}

void Scene::configureLight(const glm::vec3& position, const glm::vec3& color, float constant, float linear, float quadratic)
{
	light.setPosition(position);
	light.setColor(color);
	light.setAttenuation(constant, linear, quadratic);
}

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

Material& Scene::addMaterial()
{
    materials.push_back(std::make_unique<Material>());

    return *materials.back();
}

ShaderProgram& Scene::addShaderProgram(const Shader& vertexShader, const Shader& fragmentShader, bool useCamera, bool useLighting)
{
    auto program = std::make_unique<ShaderProgram>();
    program->link(vertexShader, fragmentShader);

    shaderPrograms.push_back(std::move(program));

    ShaderProgram& result = *shaderPrograms.back();

	if (useCamera)
	{
        camera.subscribe(result);
	}

    if (useLighting) {
        light.subscribe(result);
    }

    return result;
}

DrawableObject& Scene::addObject(Model& model, ShaderProgram& shaderProgram, Material& material)
{
    objects.push_back(std::make_unique<DrawableObject>(model, shaderProgram, material));
    return *objects.back();
}

void Scene::draw(double timeSeconds)
{
    for (const auto& program : shaderPrograms)
    {
        program->updateTimeUniform();
    }

    for (const auto& object : objects)
    {
        object->draw(timeSeconds);
    }
}

void Scene::clear()
{
    for (const auto& program : shaderPrograms) {
        camera.unsubscribe(*program);
		light.unsubscribe(*program);
    }

    objects.clear();
	materials.clear();
    models.clear();
    shaderPrograms.clear();
}

Scene::~Scene()
{
	clear();
}
