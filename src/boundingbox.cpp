#include "boundingbox.h"

glm::vec3  BoundingBox::getCorner000() const {
	return glm::vec3(min.x, min.y, min.z);
	
}

glm::vec3 BoundingBox::getCorner001() const {
	return glm::vec3(min.x, min.y, max.z);

}

glm::vec3 BoundingBox::getCorner010() const {
	return glm::vec3(min.x, max.y, min.z);

}

glm::vec3 BoundingBox::getCorner011() const {
	return glm::vec3(min.x, max.y, max.z);

}

glm::vec3 BoundingBox::getCorner100() const {
	return glm::vec3(max.x, min.y, min.z);

}

glm::vec3 BoundingBox::getCorner101() const {
	return glm::vec3(max.x, min.y, max.z);

}

glm::vec3 BoundingBox::getCorner110() const {
	return glm::vec3(max.x, max.y, min.z);

}

glm::vec3 BoundingBox::getCorner111() const {
	return glm::vec3(max.x, max.y, max.z);

}

std::vector<glm::vec3> BoundingBox::getLineSegments()
{
	std::vector<glm::vec3> seg;
	//bottom
	seg.push_back(glm::vec3(min.x, min.y, min.z));
	seg.push_back(glm::vec3(min.x, min.y, max.z));
	
	seg.push_back(glm::vec3(min.x, min.y, max.z));
	seg.push_back(glm::vec3(max.x, min.y, max.z));
	
	seg.push_back(glm::vec3(max.x, min.y, max.z));
	seg.push_back(glm::vec3(max.x, min.y, min.z));
	
	seg.push_back(glm::vec3(max.x, min.y, min.z));
	seg.push_back(glm::vec3(min.x, min.y, min.z));
	//top
	seg.push_back(glm::vec3(min.x, max.y, min.z));
	seg.push_back(glm::vec3(min.x, max.y, max.z));

	seg.push_back(glm::vec3(min.x, max.y, max.z));
	seg.push_back(glm::vec3(max.x, max.y, max.z));

	seg.push_back(glm::vec3(max.x, max.y, max.z));
	seg.push_back(glm::vec3(max.x, max.y, min.z));

	seg.push_back(glm::vec3(max.x, max.y, min.z));
	seg.push_back(glm::vec3(min.x, max.y, min.z));
	//sides
	seg.push_back(glm::vec3(min.x, min.y, min.z));
	seg.push_back(glm::vec3(min.x, max.y, min.z));

	seg.push_back(glm::vec3(min.x, min.y, max.z));
	seg.push_back(glm::vec3(min.x, max.y, max.z));

	seg.push_back(glm::vec3(max.x, min.y, max.z));
	seg.push_back(glm::vec3(max.x, max.y, max.z));
	
	seg.push_back(glm::vec3(max.x, min.y, min.z));
	seg.push_back(glm::vec3(max.x, max.y, min.z));
	return seg;
}

glm::vec3 BoundingBox::getCenter()
{
	return (min + max) / 2.0f;
}

BoundingBox::BoundingBox()
{
	infinity();
}

BoundingBox::~BoundingBox()
{
}

void BoundingBox::setAABB(glm::vec3 min, glm::vec3 max)
{
	this->min = min;
	this->max = max;
}

void BoundingBox::setAABB(BoundingBox &aabb)
{
	this->min = aabb.min;
	this->max = aabb.max;
}

void BoundingBox::infinity() {
	min = glm::vec3(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());
	max = glm::vec3(-std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity());
}

void BoundingBox::extend(const glm::vec3 &point) {
	min = glm::vec3(minF(min.x, point.x), minF(min.y, point.y), minF(min.z, point.z));
	max = glm::vec3(maxF(max.x, point.x), maxF(max.y, point.y), maxF(max.z, point.z));
}

void BoundingBox::mul(const glm::mat4 &transform) {
	float x0 = min.x, y0 = min.y, z0 = min.z, x1 = max.x, y1 = max.y, z1 = max.z;
	infinity();
	//Transform the initial box's points and then make them fit inside a new AABB
	extend(transform * glm::vec4(x0, y0, z0, 1));
	extend(transform * glm::vec4(x0, y0, z1, 1));
	extend(transform * glm::vec4(x0, y1, z0, 1));
	extend(transform * glm::vec4(x0, y1, z1, 1));
	extend(transform * glm::vec4(x1, y0, z0, 1));
	extend(transform * glm::vec4(x1, y0, z1, 1));
	extend(transform * glm::vec4(x1, y1, z0, 1));
	extend(transform * glm::vec4(x1, y1, z1, 1));
	
}

void BoundingBox::findMinMax(std::vector<glm::vec3>& points) {
	infinity();
	for (int i = 0; i < points.size(); i++)
	{
		extend(points[i]);
	}
	//qDebug() << "AABB - min:" << min.x << "," << min.y << "," << min.z << " max:" << max.x << "," << max.y << "," << max.z;
}

void BoundingBox::findMinMax(std::vector<Vertex>& vertices) {
	infinity();
	for (int i = 0; i < vertices.size(); i++)
	{
		extend(vertices[i].Position);
	}
	//qDebug() << "AABB - min:" << min.x << "," << min.y << "," << min.z << " max:" << max.x << "," << max.y << "," << max.z;
}

float BoundingBox::minF(float a, float b) {
	return a > b ? b : a;
}

float BoundingBox::maxF(float a, float b) {
	return a > b ? a : b;
}

float BoundingBox::getWidth() {
	return max.x - min.x;
}
float BoundingBox::getHeight() {
	return max.y - min.y;
}
float BoundingBox::getLength() {
	return max.z - min.z;
}
