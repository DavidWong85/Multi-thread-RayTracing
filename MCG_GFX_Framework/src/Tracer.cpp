#include "Tracer.h"

#include <iostream>

glm::vec3 Tracer::getColour(Ray _ray, int _bounce)
{
	bool hit = false; //Check did the ray hit any object
	float cloest_distance; //cloest distance between ray origin and interseciton point
	float distance; //temp distance
	glm::vec3 intersectionPt; //cloest Intersection Point
	int id; //cloest intersection sphere
	int distantFactor = 1500;

	//Check intersection for all sphere in the scene
	for (int i = 0; i < vectors_of_sphere.size(); i++)
	{
		intersection intersectionResult = vectors_of_sphere.at(i)->getIntersect(_ray);
		if (intersectionResult.intersect == true)
		{
			distance = glm::length(intersectionResult.intersection_point - _ray.origin);
			if (hit == false)
			{
				cloest_distance = distance;
				id = i;
				intersectionPt = intersectionResult.intersection_point;
				hit = true;
			}
			else
			{
				if (distance < cloest_distance)
				{
					cloest_distance = distance;
					id = i;
					intersectionPt = intersectionResult.intersection_point;
				}
			}
		}
	}

	glm::vec3 colour(0, 0, 0);
	
	if (hit == true)
	{
		//check if any other object between the intersection point and the light source
		//Set colour to black if true (shadow)
		Ray shadowRay;
		for (int i = 0; i < vectors_of_light.size(); i++)
		{
			shadowRay.origin = intersectionPt;
			shadowRay.direction = glm::normalize(vectors_of_light.at(i)->getLightPos() - shadowRay.origin);
			shadowRay.origin += shadowRay.direction * 0.001f;
			bool shadowHit = false;
			for (int j = 0; j < vectors_of_sphere.size(); j++)
			{
				intersection intersectionResult = vectors_of_sphere.at(j)->getIntersect(shadowRay);
				if (intersectionResult.intersect == true)
				{
					shadowHit = true;
				}
			}
			if (shadowHit == false)
			{
				colour += vectors_of_sphere.at(id)->shading(_ray, intersectionPt, vectors_of_light.at(i)) * (1 / (((1 + (cloest_distance / distantFactor)) * (1 + (cloest_distance / distantFactor)))));
			}
		}

		//Using recursion to calculate reflection
		if (_bounce < 3)
		{
			_bounce++;
			Ray newRay;
			newRay.origin = intersectionPt;
			newRay.direction = glm::normalize(_ray.direction - 2.0f * glm::dot(_ray.direction, vectors_of_sphere.at(id)->getNormal(intersectionPt)) * vectors_of_sphere.at(id)->getNormal(intersectionPt));
			newRay.origin += newRay.direction * 0.001f;
			colour += getColour(newRay, _bounce) * 0.6f;
			colour = colour * (1 / (((1 + (cloest_distance / distantFactor)) * (1 + (cloest_distance / distantFactor)))));
		}
	}
	colour = glm::min(colour, glm::vec3(1, 1, 1));
	return colour;
}

void Tracer::addSphere(std::shared_ptr<Sphere> _sphere)
{
	vectors_of_sphere.push_back(_sphere);
}

void Tracer::addLight(std::shared_ptr<Light> _light)
{
	vectors_of_light.push_back(_light);
}