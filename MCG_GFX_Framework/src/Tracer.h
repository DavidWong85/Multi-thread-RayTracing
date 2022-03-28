#include "Ray.h"
#include "Sphere.h"
#include "Light.h"

#include <GLM/glm.hpp>
#include <Memory>
#include <vector>

class Tracer
{
public:
	glm::vec3 getColour(Ray _ray, int _bounce);
	void addSphere(std::shared_ptr<Sphere> _sphere);
	void addLight(std::shared_ptr<Light> _light);

private:
	std::vector<std::shared_ptr<Sphere>> vectors_of_sphere;
	std::vector<std::shared_ptr<Light>> vectors_of_light;
};