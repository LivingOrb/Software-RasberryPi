#include "matrix.hpp"

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
