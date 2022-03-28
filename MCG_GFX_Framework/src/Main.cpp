#include "MCG_GFX_Lib.h"
#include "Camera.h"
#include "Tracer.h"
#include "Light.h"
#include "Tasks.h"
#include "stb_image.h"

#include <memory>
#include <iostream>
#include <vector>
#include <SDL/SDL.h>

Uint32 StartTicks;
int frame = 0;
const int fps = 60;

void StartTime()
{
	StartTicks = SDL_GetTicks();
}
int getTicks()
{
	return SDL_GetTicks() - StartTicks;
}

void draw(int _threadID, int _NumThread, std::shared_ptr<Camera> _camera, std::shared_ptr<Tracer> _tracer)
{
	glm::ivec2 pixelPosition;
	glm::ivec2 windowSize = _camera->getwindowSize();
	int bounce = 0;

	for (int x = 0; x < windowSize.x; x++)
	{
		//Each thread have a different start point, and will skip to the next working line accoring to the number of threads
		for (int y = _threadID; y < windowSize.y; y += _NumThread) 
		{
			pixelPosition.x = x;
			pixelPosition.y = y;
			glm::ivec2 currentPixel(pixelPosition.x, pixelPosition.y);

			Ray currentRay = _camera->getRay(currentPixel);
			glm::vec3 colour = _tracer->getColour(currentRay, bounce);
			//Store colour data per pixel
			MCG::DrawPixel(pixelPosition, colour * 255.0f);
		}
	}
};

int main( int argc, char *argv[] )
{
	glm::ivec2 windowSize(1000, 800);
	if( !MCG::Init(windowSize))
	{
		return -1;
	}

	MCG::SetBackground( glm::ivec3(0,0,0) );

	std::shared_ptr<Camera> camera = std::make_shared<Camera>();
	std::shared_ptr<Tracer> tracer = std::make_shared<Tracer>();

#pragma region Sphere & Light Creation
	glm::vec3 centre(-100, -60, -250);
	float radius = 50;
	glm::vec3 colour(0.7, 0.7, 0.7);

	glm::vec3 centre2(-50, 0, -170);
	float radius2 = 15;
	glm::vec3 colour2(0, 0.3, 0.5);

	glm::vec3 centre3(0, -20, -130);
	float radius3 = 15;
	glm::vec3 colour3(0.3, 0.7, 0.1);

	glm::vec3 centre4(20, 10, -150);
	float radius4 = 15;
	glm::vec3 colour4(0.6, 0, 0.1);

	glm::vec3 centre7(30, 40, -150);
	float radius7 = 15;
	glm::vec3 colour7(0.2, 0.2, 0.6);

	glm::vec3 centre8(-65, 45, -170);
	float radius8 = 15;
	glm::vec3 colour8(0.6, 0, 0.1);

	std::shared_ptr<Sphere> sphere1 = std::make_shared<Sphere>(centre, radius, colour);
	std::shared_ptr<Sphere> sphere2 = std::make_shared<Sphere>(centre2, radius2, colour2);
	std::shared_ptr<Sphere> sphere3 = std::make_shared<Sphere>(centre3, radius3, colour3);
	std::shared_ptr<Sphere> sphere4 = std::make_shared<Sphere>(centre4, radius4, colour4);
	std::shared_ptr<Sphere> sphere7 = std::make_shared<Sphere>(centre7, radius7, colour7);
	std::shared_ptr<Sphere> sphere8 = std::make_shared<Sphere>(centre8, radius8, colour8);

	glm::vec3 lightPos(-50, 0, 0); 
	glm::vec3 lightColour(0.7, 1, 0.3);

	glm::vec3 lightPos2(50, 0, 0);
	glm::vec3 lightColour2(1, 0.5, 0.9);

	std::shared_ptr<Light> light1 = std::make_shared<Light>(lightPos, lightColour);
	std::shared_ptr<Light> light2 = std::make_shared<Light>(lightPos2, lightColour2);

#pragma endregion

	//Adding Sphere and Light in the scene
	tracer->addSphere(sphere1);
	tracer->addSphere(sphere2);
	tracer->addSphere(sphere3);
	tracer->addSphere(sphere4);
	tracer->addSphere(sphere7);
	tracer->addSphere(sphere8);
	tracer->addLight(light1);
	tracer->addLight(light2);

	bool change = false;
	bool finished = false;

	//User input number of threads: 
	int numofthread;
	std::cout << "Input number of thread(s) : ";
	std::cin >> numofthread;

	//create thread pool:
	Tasks tasks;
	tasks.start(numofthread);

	while (MCG::ProcessFrame() && finished != true)
	{
		StartTime();
		MCG::SetBackground(glm::ivec3(0, 0, 0));
		//Add task to the thread pool:
		for (int i = 0; i < numofthread; i++)
		{
			tasks.queue(
				[i, numofthread, camera, tracer]
				{
					draw(i, numofthread, camera, tracer);
				});
		}
		//Wait all thread finish their work (Not working Properly)
		tasks.waitFinished();
		//use the collected data to create a surface and texture
		MCG::createFrameBuffer();
		MCG::createTexture();
		//Display the Texture 
		SDL_RenderCopy(MCG::getRenderer(), MCG::getTexture(), NULL, NULL);
		//Distroy the Texture to save memory
		SDL_DestroyTexture(MCG::getTexture());
		SDL_FreeSurface(MCG::getSurface());

		frame++;
		//Frame limiter
		if (getTicks() < 1000 / fps)
		{
			//SDL_Delay((1000 / fps) - getTicks());
		}
		//Move Sphere 1 
		if (sphere1->getCentre().x < 100 && change == false)
		{
			sphere1->setCentre(sphere1->getCentre() + glm::vec3(1, 0, 0));
			if (sphere1->getCentre().x > 80)
			{
				change = true;
			}
		}
		if (change == true)
		{
			sphere1->setCentre(sphere1->getCentre() + glm::vec3(-1, 1, 0.75));
			if (sphere1->getRadius() > 20)
			{
				sphere1->setRadius(sphere1->getRadius() - 0.5);
			}
		}
		if (sphere1->getCentre().x < -20 && change == true)
		{
			std::cout << "Demo End" << std::endl;
			finished = true;
		}
	}
	MCG::ShowAndHold();
	return 0;
}