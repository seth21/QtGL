#ifndef CUBEOBSERVER_H
#define CUBEOBSERVER_H

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
class CubeObserver {
public:
	
	static std::vector<glm::mat4> getProjectionViewMatrices(const glm::vec3& cubePos, float viewRadius) {
		std::vector<glm::mat4> projView;

		m_near = 0.1f;
		m_far = viewRadius;
		glm::mat4x4 proj = glm::perspective(glm::radians(90.0f), 1.0f, m_near, m_far);

		projView.push_back(proj * glm::lookAt(cubePos, cubePos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		projView.push_back(proj * glm::lookAt(cubePos, cubePos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		projView.push_back(proj * glm::lookAt(cubePos, cubePos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
		projView.push_back(proj * glm::lookAt(cubePos, cubePos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
		projView.push_back(proj * glm::lookAt(cubePos, cubePos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
		projView.push_back(proj * glm::lookAt(cubePos, cubePos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
		return projView;
	}

};

#endif