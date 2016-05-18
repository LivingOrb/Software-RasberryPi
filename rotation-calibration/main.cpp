#include <iostream>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

#include <glm/matrix.hpp>
#include <glm/vec3.hpp>
#include <common/matrix.hpp>
#include <common/GY85.hpp>
#include <common/Timer.hpp>

static pthread_mutex_t mutex;
static pthread_t thread;
static GY85 gy85;
static Timer timer;

static bool running;
static void *measure(void *arg)
{
	while (running)
	{
		if (!pthread_mutex_lock(&mutex))
		{
			float dt = timer.getElapsedTime();
			gy85.update(dt);

			pthread_mutex_unlock(&mutex);
		}

		usleep(30000);
	}

}

static int shutdown(int code)
{
	running = false;
	void *ret;
	pthread_join(thread, &ret);
	return code;
}

int main()
{
	if (pthread_mutex_init(&mutex, nullptr))
	{
		return -1;
	}

	if (!gy85.initialize())
	{
		return -1;
	}

	if (!timer.initialize())
	{
		return -1;
	}

	running = true;

	pthread_create(&thread, nullptr, measure, nullptr);

	std::cout << "Define the coordinate system and press Enter" << std::endl;
	std::cin.get();

	if (pthread_mutex_lock(&mutex))
	{
		return shutdown(-1);
	}

	glm::vec3 accelerometer;
	gy85.getAccelerometer(accelerometer);

	glm::vec3 compass;
	gy85.getCompass(compass);

	glm::mat3 cardToWorld;
	computeToWorldMatrix(accelerometer, compass, cardToWorld);

	glm::mat3 worldToCard = glm::inverse(cardToWorld);
	saveMatrix("rotation.calibration", worldToCard);

	pthread_mutex_unlock(&mutex);

	return shutdown(0);
}
