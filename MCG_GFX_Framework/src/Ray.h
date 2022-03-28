#ifndef _RAY_H_
#define _RAY_H_

#include <GLM/glm.hpp>

struct Ray
{
	glm::vec3 origin; //pixel position
	glm::vec3 direction; //(0,0,-1)
};
#endif