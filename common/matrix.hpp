#pragma once

#include <glm/mat3x3.hpp>
#include <glm/vec3.hpp>

void computeToWorldMatrix(const glm::vec3 &accelerometer, const glm::vec3 &compass, glm::mat3 &matrix);

bool loadMatrix(const char *filename, glm::mat3 &matrix);
void saveMatrix(const char *filename, const glm::mat3 &matrix);
