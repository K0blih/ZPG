#include "Light.h"
#include "ILightObserver.h"

#include <algorithm>
#include <stdexcept>

void Light::subscribe(ILightObserver& observer)
{
	const auto it = std::find(observers.begin(), observers.end(), &observer);

	if (it == observers.end()) {
		observers.push_back(&observer);
	}

	observer.onLightChanged(position, color, attenuationConstant, attenuationLinear, attenuationQuadratic);
}

void Light::unsubscribe(ILightObserver& observer)
{
	std::erase(observers, &observer);
}

void Light::setPosition(const glm::vec3& value)
{
	position = value;
	notify();
}

void Light::setColor(const glm::vec3& value)
{
	color = value;
	notify();
}

void Light::setAttenuation(float constant, float linear, float quadratic)
{
	if (constant <= 0.0f || linear < 0.0f || quadratic < 0.0f) {
		throw std::invalid_argument("Attenuation requires constant > 0, linear >= 0, quadratic >= 0");
	}

	attenuationConstant = constant;
	attenuationLinear = linear;
	attenuationQuadratic = quadratic;
	notify();
}

void Light::notify()
{
	for (ILightObserver* observer : observers) {
		observer->onLightChanged(position, color, attenuationConstant, attenuationLinear, attenuationQuadratic);
	}
}
