#include <fstream>
#include <iostream>
#include <signal.h>
#include <unistd.h>

extern "C"
{
#include <ws281x/ws2811.h>
}

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

static const int LedCount = 102;

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
	else
	{
		std::cerr << "Warning: could not found rotation calibration" << std::endl;
	}

	ws2811_t ledstring;
	memset((char*)&ledstring, 0, sizeof(ledstring));
	ledstring.freq = WS2811_TARGET_FREQ;
	ledstring.dmanum = 5;

	ledstring.channel[0].gpionum = 18;
	ledstring.channel[0].count = LedCount;
	ledstring.channel[0].invert = 1;
	ledstring.channel[0].brightness = 255;
	ledstring.channel[0].strip_type = WS2811_STRIP_GRB;

	if (ws2811_init(&ledstring))
	{
		std::cerr << "Failed to initialize ledstring" << std::endl;
		return -1;
	}

	ws2811_led_t *leds = ledstring.channel[0].leds;

	GY85 gy85;
	if (!gy85.initialize())
	{
		std::cerr << "Failed to initialize GY-85" << std::endl;
		gy85.shutdown();
		return -1;
	}

	Timer timer;
	if (!timer.initialize())
	{
		std::cerr << "Failed to initialize timer" << std::endl;
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

		ws2811_render(&ledstring);
	}

	ws2811_render(&ledstring);

	gy85.shutdown();
	ws2811_fini(&ledstring);
	return 0;
}
