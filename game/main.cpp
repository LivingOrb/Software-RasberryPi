#include <fstream>
#include <iostream>
#include <signal.h>
#include <unistd.h>

#include <glm/mat3x3.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <common/matrix.hpp>
#include <common/GY85.hpp>
#include <common/Timer.hpp>

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

	std::ifstream rotationCalibration("rotation.calibration", std::ifstream::binary);
	if (rotationCalibration.is_open())
	{
		float matrixData[9];
		rotationCalibration.read((char*)matrixData, sizeof(matrixData));
		rotationCalibration.close();
		memcpy(glm::value_ptr(sphereToCard), matrixData, sizeof(matrixData));
	}

	GY85 gy85;
	if (!gy85.initialize())
	{
		gy85.shutdown();
		return -1;
	}

	Timer timer;
	if (!timer.initialize())
	{
		gy85.shutdown();
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

		usleep(30000);
	}

	gy85.shutdown();
    return 0;
}
