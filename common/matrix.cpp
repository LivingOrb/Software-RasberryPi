#include "matrix.hpp"

#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/perpendicular.hpp>

void computeToWorldMatrix(const glm::vec3 &accelerometer, const glm::vec3 &compass, glm::mat3 &matrix)
{
	glm::vec3 up = glm::normalize(accelerometer);
	glm::vec3 north = glm::normalize(glm::perp(compass, up));
	glm::vec3 east = glm::normalize(glm::cross(north, up));

	matrix[0] = east;
	matrix[1] = north;
	matrix[2] = up;
}

bool loadMatrix(const char *filename, glm::mat3 &matrix)
{
	std::ifstream stream(filename, std::ifstream::binary);
	if (!stream.is_open())
		return false;

	stream.read((char*)glm::value_ptr(matrix), 9 * sizeof(float));
	stream.close();

	return true;
}

void saveMatrix(const char *filename, const glm::mat3 &matrix)
{
	std::ofstream stream(filename, std::ofstream::binary);
	stream.write((char*)glm::value_ptr(matrix), 9 * sizeof(float));
	stream.close();
}
