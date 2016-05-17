#pragma once

#include <common/Coordinate.hpp>

class GY85
{
public:
	static const int MaxAdapters;

	GY85();

	bool initialize();
	void shutdown();

	void update(float dt);

	void getAccelerometer(glm::vec3 &vec) const;
	void getCompass(glm::vec3 &vec) const;

protected:
	bool aConfigure();
	bool cConfigure();

private:
	int aFile;
	int cFile;

	Coordinate aX, aY, aZ;
	Coordinate cX, cY, cZ;
};
