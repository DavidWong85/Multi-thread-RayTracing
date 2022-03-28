#include "Camera.h"

Ray Camera::getRay(glm::ivec2 coordiante)
{
	Ray _ray;
	
	glm::vec2 NDCcoordinate((glm::vec2(coordiante) / windowSize) * 2.0f - 1.0f);
	glm::vec4 Near(NDCcoordinate, -1, 1);
	glm::vec4 Far(NDCcoordinate, 1, 1);

	Near = invProjMat * Near;
	Far = invProjMat * Far;

	Near = Near / Near.w;
	Far = Far / Far.w;

	Near = invViewMat * Near;
	Far = invViewMat * Far;

	_ray.origin = glm::vec3(Near);
	_ray.direction = glm::normalize(Far - Near);

	return _ray;
}