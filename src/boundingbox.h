#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H
#include <glm/vec3.hpp>
#include "vertex.h"
#include <vector>
#include <limits>
#include <glm/mat4x4.hpp>
#include <QDebug>

class BoundingBox {
public:
	BoundingBox();
	~BoundingBox();
	void setAABB(glm::vec3 min, glm::vec3 max);
	void setAABB(BoundingBox& aabb);
	glm::vec3 getCorner000() const;
	glm::vec3 getCorner001() const;
	glm::vec3 getCorner010() const;
	glm::vec3 getCorner011() const;
	glm::vec3 getCorner100() const;
	glm::vec3 getCorner101() const;
	glm::vec3 getCorner110() const;
	glm::vec3 getCorner111() const;
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

	float minF(float a, float b);
	float maxF(float a, float b);
};

#endif