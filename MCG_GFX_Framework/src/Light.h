#ifndef _LIGHT_H_
#define _LIGHT_H_

#include <GLM/glm.hpp>

class Light
{
public:
	Light(glm::vec3 _lightPos, glm::vec3 _lightColour);
	glm::vec3 getLightPos();
	glm::vec3 getLightColour();

private:
	glm::vec3 lightPos;
	glm::vec3 lightColour;
};

#endif