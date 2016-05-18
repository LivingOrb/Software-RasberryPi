#include "Leds.hpp"

#include <cstring>

const int Leds::Count = 102;

Leds::Leds()
	: ledColors(nullptr)
	, ledPositions(nullptr)
{}

Leds::~Leds()
{
	if (ledColors)
	{
		delete ledPositions;
		memset((char*)ledColors, 0, Count * sizeof(ws2811_led_t));
		ws2811_render(&ledstring);
		ws2811_fini(&ledstring);
	}
}

bool Leds::initialize()
{
	memset((char*)&ledstring, 0, sizeof(ledstring));
	ledstring.freq = WS2811_TARGET_FREQ;
	ledstring.dmanum = 5;

	ledstring.channel[0].gpionum = 18;
	ledstring.channel[0].count = Count;
	ledstring.channel[0].invert = 1;
	ledstring.channel[0].brightness = 255;
	ledstring.channel[0].strip_type = WS2811_STRIP_GRB;

	if (ws2811_init(&ledstring))
	{
		return false;
	}

	ledColors = ledstring.channel[0].leds;

	ledPositions = new glm::vec3[Count];

	return true;
}

void Leds::render()
{
	ws2811_render(&ledstring);
}
