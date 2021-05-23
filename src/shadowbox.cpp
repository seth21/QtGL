#include "shadowbox.h"


/**
	* Creates a new shadow box and calculates some initial values relating to
	* the camera's view frustum, namely the width and height of the near plane
	* and (possibly adjusted) far plane.
	*
	*/
ShadowBox::ShadowBox() {
	frustum = std::make_unique<Frustum>();
}

/**
	* Updates the bounds of the shadow box based on the light direction and the
	* camera's view frustum, to make sure that the box covers the smallest area
	* possible while still ensuring that everything inside the camera's view
	* (within a certain range) will cast shadows.
	*/
void ShadowBox::update(Camera *cam, glm::mat4 lightViewMatrix, float shadowMapWidth, float shadowMapHeight) {

	frustum->calculateFrustum(cam->FOVy, cam->nearPlane, SHADOW_DISTANCE, cam->aspect, cam->position, cam->getFront(), cam->getRight());
	//transform camera frustum points to light space
	std::vector<glm::vec3> lightSpaceFrustumPoints;
	const std::vector<glm::vec3>& frustumPoints = frustum->getFrustumPoints();
	for (int i = 0; i < frustumPoints.size(); i++) lightSpaceFrustumPoints.push_back(lightViewMatrix * glm::vec4(frustumPoints[i], 1));
	
	lightspaceBB.findMinMax(lightSpaceFrustumPoints);
	lightspaceBB.max.z += OFFSET;

	// Round the light's bounding box to the nearest texel unit to reduce flickering.
	glm::vec2 unitsPerTexel = 2.0f * glm::vec2(lightspaceBB.getWidth(), lightspaceBB.getHeight()) / glm::vec2(shadowMapWidth, shadowMapHeight);

	lightspaceBB.min.x = floor(lightspaceBB.min.x / unitsPerTexel.x) * unitsPerTexel.x;
	lightspaceBB.max.x = floor(lightspaceBB.max.x / unitsPerTexel.x) * unitsPerTexel.x;

	lightspaceBB.min.y = floor(lightspaceBB.min.y / unitsPerTexel.y) * unitsPerTexel.y;
	lightspaceBB.max.y = floor(lightspaceBB.max.y / unitsPerTexel.y) * unitsPerTexel.y;
}

/**
	* Calculates the center of the "view cuboid" in light space first, and then
	* converts this to world space using the inverse light's view matrix.
	*/
glm::vec3 ShadowBox::getCenter(glm::mat4 lightViewMatrix) {
	glm::vec3 lsBBCenter = lightspaceBB.getCenter();
	glm::mat4x4 invertedLight = glm::inverse(lightViewMatrix);
	glm::vec3 worldLSBBCenter = invertedLight * glm::vec4(lsBBCenter, 1);
	return worldLSBBCenter;
}

/**
	* @return The width of the "view cuboid" (orthographic projection area).
	*/
float ShadowBox::getWidth() {
	return lightspaceBB.getWidth();
}

/**
	* @return The height of the "view cuboid" (orthographic projection area).
	*/
float ShadowBox::getHeight() {
	return lightspaceBB.getHeight();
}

/**
	* @return The length of the "view cuboid" (orthographic projection area).
	*/
float ShadowBox::getLength() {
	return lightspaceBB.getLength();
}

glm::vec3 ShadowBox::getMin() {
	return lightspaceBB.min;
}

glm::vec3 ShadowBox::getMax() {
	return lightspaceBB.max;
}

ShadowBox::~ShadowBox()
{
}

Frustum* ShadowBox::getFrustum()
{
	return frustum.get();
}


