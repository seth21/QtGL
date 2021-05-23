#include "frustum.h"

Frustum::Frustum()
{
	for (int i = 0; i < 6; i++) {
		planes[i] = std::make_unique<Plane>();
	}
}

Frustum::~Frustum()
{
}

void Frustum::calculateFrustum(const float fovYdegrees, const float near, const float far, const float aspect, const glm::vec3 &camPos, const glm::vec3 &camFront, const glm::vec3 &camRight)
{
	float fovYrads = glm::radians(fovYdegrees);
	float Hnear = 2.0f * tan(fovYrads / 2.0f) * near;
	float Wnear = Hnear * aspect;
	float Hfar = 2.0f * tan(fovYrads / 2.0f) * far;
	float Wfar = Hfar * aspect;
	//qDebug() << "Hfar/near:" << Hfar << "," << Hnear;
	glm::vec3 up = -glm::cross(camFront, camRight);
	glm::vec3 fc = camPos + camFront * far;
	//qDebug() << "fc:" << fc.x << "," << fc.y << "," << fc.z;
	glm::vec3 ftl = fc + (up * Hfar / 2.0f) - (camRight * Wfar / 2.0f);
	glm::vec3 ftr = fc + (up * Hfar / 2.0f) + (camRight * Wfar / 2.0f);
	glm::vec3 fbl = fc - (up * Hfar / 2.0f) - (camRight * Wfar / 2.0f);
	glm::vec3 fbr = fc - (up * Hfar / 2.0f) + (camRight * Wfar / 2.0f);

	glm::vec3 nc = camPos + camFront * near;
	glm::vec3 ntl = nc + (up * Hnear / 2.0f) - (camRight * Wnear / 2.0f);
	glm::vec3 ntr = nc + (up * Hnear / 2.0f) + (camRight * Wnear / 2.0f);
	glm::vec3 nbl = nc - (up * Hnear / 2.0f) - (camRight * Wnear / 2.0f);
	glm::vec3 nbr = nc - (up * Hnear / 2.0f) + (camRight * Wnear / 2.0f);

	// compute the six planes
	// the function set3Points assumes that the points
	// are given in counter clockwise order
	planes[TOP]->set3Points(ntr, ntl, ftl);
	planes[BOTTOM]->set3Points(nbl, nbr, fbr);
	planes[LEFT]->set3Points(ntl, nbl, fbl);
	planes[RIGHT]->set3Points(nbr, ntr, fbr);
	planes[NEARP]->set3Points(ntl, ntr, nbr);
	planes[FARP]->set3Points(ftr, ftl, fbl);

	frustumPoints.clear();
	frustumPoints.push_back(ntl);
	frustumPoints.push_back(ntr);
	frustumPoints.push_back(nbr);
	frustumPoints.push_back(nbl);
	frustumPoints.push_back(ftl);
	frustumPoints.push_back(ftr);
	frustumPoints.push_back(fbr);
	frustumPoints.push_back(fbl);
}

BoundingBox Frustum::getFrustumAABB() {
	BoundingBox aabb;
	aabb.findMinMax(frustumPoints);
	return aabb;
}

bool Frustum::intersects()
{
	return false;
}

bool Frustum::pointInFrustum(glm::vec3& p)
{
	for (int i = 0; i < 6; i++) {
		if (planes[i]->testPoint(p) == Back)
			return false;
	}
	return true;
}

bool Frustum::sphereInFrustum(const glm::vec3& p, float radius) {

	float distance;

	for (int i = 0; i < 6; i++) {
		distance = planes[i]->distance(p);
		if (distance < -radius)
			return false;
	}
	return true;
}

bool Frustum::sphereInFrustum(const BoundingSphere& sphere)
{
	return sphereInFrustum(sphere.m_center, sphere.m_radius);
}

bool Frustum::boundsInFrustum(const BoundingBox &bounds) {
	glm::vec3 tmpV;
	auto &corners = bounds.getCorners();
	//qDebug() << "Corner000: " << tmpV.x << "," << tmpV.y << "," << tmpV.z;
	//qDebug() << "Plane:" << (planes[1]->normal.x) << "," << (planes[1]->normal.y) <<","<< (planes[1]->normal.z);
	for (int i = 0; i < 6; i++) {
		bool allBack = true;
		for (int j = 0; j < corners.size(); j++) {
			if (planes[i]->testPoint(corners[j]) == PlaneSide::Front) {
				allBack = false;
				break;
			}
		}
		if (allBack) return false;
	}

	return true;
}

std::vector<glm::vec3> Frustum::getLineSegments()
{
	std::vector<glm::vec3> seg;
	seg.push_back(frustumPoints[0]);
	seg.push_back(frustumPoints[1]);
	seg.push_back(frustumPoints[1]);
	seg.push_back(frustumPoints[2]);
	seg.push_back(frustumPoints[2]);
	seg.push_back(frustumPoints[3]);
	seg.push_back(frustumPoints[3]);
	seg.push_back(frustumPoints[0]);

	seg.push_back(frustumPoints[4]);
	seg.push_back(frustumPoints[5]);
	seg.push_back(frustumPoints[5]);
	seg.push_back(frustumPoints[6]);
	seg.push_back(frustumPoints[6]);
	seg.push_back(frustumPoints[7]);
	seg.push_back(frustumPoints[7]);
	seg.push_back(frustumPoints[4]);

	seg.push_back(frustumPoints[0]);
	seg.push_back(frustumPoints[4]);
	seg.push_back(frustumPoints[1]);
	seg.push_back(frustumPoints[5]);
	seg.push_back(frustumPoints[2]);
	seg.push_back(frustumPoints[6]);
	seg.push_back(frustumPoints[3]);
	seg.push_back(frustumPoints[7]);
	return seg;

}

const std::vector<glm::vec3>& Frustum::getFrustumPoints()
{
	return frustumPoints;
}
