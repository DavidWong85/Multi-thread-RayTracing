#include "Sphere.h"

#include <cmath>
#include <iostream>

Sphere::Sphere(glm::vec3 _centre, float _radius, glm::vec3 _colour)
{
	centre = _centre;
	radius = _radius;
	colour = _colour;
}

Sphere::~Sphere() {}
 
intersection Sphere::getIntersect(Ray _ray)
{
	intersection _intersect;
	float distance = glm::length(_ray.origin - centre);
	if (distance < radius)
	{
		_intersect.intersect = false;
	}
	else
	{
		glm::vec3 query_point = centre;
		glm::vec3 ray_direction = _ray.direction;
		glm::vec3 position_vector(query_point - _ray.origin);
		float dotResult = glm::dot(position_vector, ray_direction);
		if (dotResult < 0)
		{
			_intersect.intersect = false;
		}
		else
		{
			float distance2 = glm::length(position_vector - (dotResult * ray_direction));

			if (distance2 <= radius)
			{ 
				float x = sqrt((radius * radius) - (distance2 * distance2));
				_intersect.intersection_point = _ray.origin + (((dotResult - x)) * ray_direction);
				_intersect.intersect = true;
			}
			else
			{
				_intersect.intersect = false;
			}
		}
	}
	return _intersect;
}

glm::vec3 Sphere::getNormal(glm::vec3 _intersectionPt)
{
	glm::vec3 normal(_intersectionPt - centre);
	normal = glm::normalize(normal);
	return normal;
}

glm::vec3 Sphere::shading(Ray _ray, glm::vec3 _intersectionPt, std::shared_ptr<Light> _light)
{
	glm::vec3 lightdirection = glm::normalize(_light->getLightPos() - _intersectionPt);
	return (glm::max(0.0f, glm::dot(lightdirection, getNormal(_intersectionPt))) * _light->getLightColour() * colour); //colour after shading
}

glm::vec3 Sphere::getCentre()
{
	return centre;
}

void Sphere::setCentre(glm::vec3 _centre)
{
	centre = _centre;
}

float Sphere::getRadius()
{
	return radius;
}

void Sphere::setRadius(float _radius)
{
	radius = _radius;
}

glm::vec3 Sphere::getColour()
{
	return colour;
}