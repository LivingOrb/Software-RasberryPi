#include <algorithm>
#include <iostream>
#include <signal.h>
#include <unistd.h>

#include <glm/mat3x3.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/string_cast.hpp>
#include <common/color.hpp>
#include <common/Leds.hpp>
#include <common/matrix.hpp>
#include <common/GY85.hpp>
#include <common/Timer.hpp>
#include <game/Mode.hpp>

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

struct HSV
{
	float H, S, V;
};

static Leds leds;
/*
static int currentLed, targetLed;
static float maxCountdown, countdown;
static float hue;
static std::vector<HSV> ledHSV(Leds::Count, {0.0f, 0.0f, 0.0f});

static const std::vector<int> &findHighestLeds()
{
	std::sort(leds.allIndices.begin(), leds.allIndices.end(), [](int a, int b)
	{
		return leds.worldPositions[a].z > leds.worldPositions[b].z;
	});
	return leds.allIndices;
}

static const std::vector<int> &findLowestNeighbors(int led)
{
	std::vector<int> &neighbors = leds.neighbors[led];
	std::sort(neighbors.begin(), neighbors.end(), [](int a, int b)
	{
		return leds.worldPositions[a].z < leds.worldPositions[b].z;
	});
	return neighbors;
}

void turnOnCurrentLed()
{
	ledHSV[currentLed].H = hue;
	ledHSV[currentLed].S = 1.0;
	ledHSV[currentLed].V = 1.0;
}
*/
int main(void)
{
	glm::mat3 sphereToCard;
	if (!loadMatrix("rotation.calibration", sphereToCard))
	{
		std::cerr << "Warning: could not found rotation calibration" << std::endl;
	}

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

	Mode mode(leds);
	if (!mode.loadScript())
	{
		std::cerr << "Failed to load script" << std::endl;
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

		glm::mat3 worldToCard;
		computeToWorldMatrix(accelerometer, compass, worldToCard);
		glm::mat3 cardToWorld = glm::inverse(worldToCard);

		glm::mat3 sphereToWorld = cardToWorld * sphereToCard;
		leds.updateWorldPositions(sphereToWorld);

		leds.clear();

		if (mode.update(dt))
		{
			leds.render();
		}
	}

	return 0;
}
