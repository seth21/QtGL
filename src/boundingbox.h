#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H
#include <glm/vec3.hpp>
#include "vertex.h"
#include <vector>
#include <limits>
#include <glm/mat4x4.hpp>
#include <QDebug>

enum AABBPoints {
	LBB,
	LBF,
	LTB,
	LTF,
	RBB,
	RBF,
	RTB,
	RTF,
	COUNT
};

class BoundingBox {
public:
	BoundingBox();
	~BoundingBox();
	void setAABB(glm::vec3 min, glm::vec3 max);
	void setAABB(BoundingBox& aabb);
	const std::vector<glm::vec3>& getCorners() const;
	void calculateCorners();
	glm::vec3 min;
	glm::vec3 max;
	void findMinMax(std::vector<Vertex> &vertices);
	void findMinMax(std::vector<glm::vec3>& points);
	void infinity();
	void extend(const glm::vec3 &point);
	void mul(const glm::mat4 &transform);
	std::vector<glm::vec3> getLineSegments();
	glm::vec3 getCenter();
	float getWidth();
	float getHeight();
	float getLength();
private:
	std::vector<glm::vec3> corners;
	float minF(float a, float b);
	float maxF(float a, float b);
};

#endif