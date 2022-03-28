#include "Ray.h"
#include "Intersection.h"
#include "Light.h"

#include <GLM/glm.hpp>
#include <Memory>

class Sphere
{
public:
	Sphere(glm::vec3 _centre, float _radius, glm::vec3 _colour);
	~Sphere();
	intersection getIntersect(Ray _ray);
	glm::vec3 getNormal(glm::vec3 _intersectionPt);
	glm::vec3 shading(Ray _ray, glm::vec3 _intersectionPt, std::shared_ptr<Light> _light);

	glm::vec3 getCentre();
	void setCentre(glm::vec3 _centre);
	float getRadius();
	void setRadius(float _radius);
	glm::vec3 getColour();

private:
	glm::vec3 centre;
	float radius;
	glm::vec3 colour;

};