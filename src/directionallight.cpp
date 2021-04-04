#include "directionallight.h"

DirectionalLight::DirectionalLight()
{
	shadowMap = std::make_unique<FrameBuffer>(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);
	//ONLY DEPTH
	shadowMap->registerDepthAttachment(GL_FLOAT, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_LINEAR);
	shadowMap->setRenderTargets(0);
	shadowMap->setup();
	shadowBox = std::make_unique<ShadowBox>();
	offset = createOffset();
	setDirection(glm::vec3(0, -1, 0.2f));
	setColor(glm::vec3(0.5, 0.5, 0.5));
	lightViewMatrix = glm::mat4(1);
}

DirectionalLight::~DirectionalLight()
{
}

/**
	 * This biased projection-view matrix is used to convert fragments into
	 * "shadow map space" when rendering the main render pass. It converts a
	 * world space position into a 2D coordinate on the shadow map.
	 */
glm::mat4x4 DirectionalLight::getToShadowMapSpaceMatrix() {
	return shadowMapSpaceMatrix;
}

/**
	 * @return The light's "view" matrix.
	 */
glm::mat4x4 DirectionalLight::getLightViewMatrix() {
	return lightViewMatrix;
}

glm::mat4x4 DirectionalLight::getProjectionLightViewMatrix() {
	return projectionViewMatrix;
}

glm::mat4x4 DirectionalLight::getProjectionMatrix() {
	return projectionMatrix;
}

/**
	 * This first updates the dimensions of
	 * the orthographic "view cuboid" based on the information that was
	 * calculated in the Shadowbox class. The light's "view" matrix is
	 * also calculated based on the light's direction and the center position of
	 * the "view cuboid" which was also calculated in the ShadowBox
	 * class. These two matrices are multiplied together to create the
	 * projection-view matrix. This matrix determines the size, position, and
	 * orientation of the "view cuboid" in the world. 
	 */
void DirectionalLight::updateMatrices(Camera* cam) {
	
	shadowBox->update(cam, lightViewMatrix);
	updateLightViewMatrix(direction, shadowBox->getCenter(lightViewMatrix));
	updateOrthoProjectionMatrix(shadowBox->getWidth(), shadowBox->getHeight(), shadowBox->getLength());

	projectionViewMatrix = projectionMatrix * lightViewMatrix;
	shadowMapSpaceMatrix = projectionViewMatrix;
}

/**
	 * Updates the "view" matrix of the light. This creates a view matrix which
	 * will line up the direction of the "view cuboid" with the direction of the
	 * light. The created view matrix determines
	 * where and how the "view cuboid" is positioned in the world. The size of
	 * the view cuboid, however, is determined by the projection matrix.
	 */
void DirectionalLight::updateLightViewMatrix(glm::vec3 dir, glm::vec3 center) {
	lightViewMatrix = glm::lookAt(center, center+dir, glm::vec3(0, 1, 0));
}

/**
 * Creates the orthographic projection matrix. This projection matrix
 * basically sets the width, length and height of the "view cuboid", based
 * on the values that were calculated in the ShadowBox class.
 */
void DirectionalLight::updateOrthoProjectionMatrix(float width, float height, float length) {
	projectionMatrix = glm::ortho(-width/2.0f, width / 2.0f, -height / 2.0f, height / 2.0f, -length / 2.0f, length / 2.0f);
}

int DirectionalLight::getShadowMapSize()
{
	return SHADOW_MAP_SIZE;
}

void DirectionalLight::setDirection(glm::vec3 dir)
{
	direction = glm::normalize(dir);
}

glm::vec3 DirectionalLight::getDirection()
{
	return direction;
}

FrameBuffer* DirectionalLight::getShadowMap()
{
	return shadowMap.get();
}

void DirectionalLight::setColor(glm::vec3 col)
{
	color = col;
}

glm::vec3 DirectionalLight::getColor()
{
	return color;
}

glm::mat4x4 DirectionalLight::createOffset() {
	glm::mat4x4 offset;
	glm::translate(offset, glm::vec3(0.5f, 0.5f, 0.5f));
	glm::scale(offset, glm::vec3(0.5f, 0.5f, 0.5f));
	return offset;
}
