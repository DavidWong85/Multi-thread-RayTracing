#include "Ray.h"

#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Memory>

class Camera
{
public:
	Ray getRay(glm::ivec2 coordiante);
	glm::vec2 getwindowSize() { return windowSize; }

private:
	glm::vec2 windowSize = glm::vec2(1000, 800);
	glm::mat4 invViewMat = glm::inverse(glm::mat4(1.0f));
	glm::mat4 invProjMat = glm::inverse(glm::perspective(glm::radians(45.0f), (float)1000 / (float)800, 0.1f, 100.0f));
};