#include "Light.h"

Light::Light(glm::vec3 _lightPos, glm::vec3 _lightColour)
{
	lightPos = _lightPos;
	lightColour = _lightColour;
}

glm::vec3 Light::getLightPos()
{
	return lightPos;
}

glm::vec3 Light::getLightColour()
{
	return lightColour;
}