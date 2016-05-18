#include <fstream>
#include <iostream>
#include <signal.h>
#include <unistd.h>

#include <glm/mat3x3.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/string_cast.hpp>
#include <common/matrix.hpp>
#include <common/GY85.hpp>
#include <common/Timer.hpp>
#include <game/Leds.hpp>

static bool running;

static void ctrlCHandler(int signum)
{
	running = false;
}

static void setupHandlers(void)
{
	struct sigaction sa = {};
	sa.sa_handler = ctrlCHandler;

	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
}

int main(void)
{
	glm::mat3 sphereToCard;
	if (!loadMatrix("rotation.calibration", sphereToCard))
	{
		std::cerr << "Warning: could not found rotation calibration" << std::endl;
	}

	Leds leds;
	if (!leds.initialize())
	{
		std::cerr << "Failed to initialize leds" << std::endl;
		return -1;
	}

	GY85 gy85;
	if (!gy85.initialize())
	{
		std::cerr << "Failed to initialize GY-85" << std::endl;
		return -1;
	}

	Timer timer;
	if (!timer.initialize())
	{
		std::cerr << "Failed to initialize timer" << std::endl;
		return -1;
	}

	running = true;
	setupHandlers();

	while (running)
	{
		float dt = timer.getElapsedTime();
		gy85.update(dt);

		glm::vec3 accelerometer;
		gy85.getAccelerometer(accelerometer);

		glm::vec3 compass;
		gy85.getCompass(compass);

		glm::mat3 cardToWorld;
		computeToWorldMatrix(accelerometer, compass, cardToWorld);

		glm::mat3 sphereToWorld = cardToWorld * sphereToCard;
		leds.updateWorldPositions(sphereToWorld);
		
		leds.render();
	}

	return 0;
}
