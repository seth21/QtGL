#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "framebuffer.h"
#include <memory>
#include "shadowbox.h"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

class DirectionalLight {
public:
	DirectionalLight();
	~DirectionalLight();
	glm::mat4x4 getToShadowMapSpaceMatrix();
	glm::mat4x4 getLightViewMatrix();
	glm::mat4x4 getProjectionLightViewMatrix();
	glm::mat4x4 getProjectionMatrix();
	
	void updateMatrices(Camera* cam);
	void setDirection(glm::vec3 dir);
	glm::vec3 getDirection();
	void setColor(glm::vec3 col);
	glm::vec3 getColor();
	FrameBuffer* getShadowMap();
	int getShadowMapSize();
	ShadowBox* getShadowBox();
private:
	std::unique_ptr<FrameBuffer> shadowMap;
	int SHADOW_MAP_SIZE = 4096;
	std::unique_ptr<ShadowBox> shadowBox;
	glm::vec3 direction;
	glm::vec3 color;
	glm::mat4x4 projectionMatrix;
	glm::mat4x4 lightViewMatrix;
	glm::mat4x4 projectionViewMatrix;
	glm::mat4x4 shadowMapSpaceMatrix;
	glm::mat4x4 offset;

	glm::mat4x4 createOffset();
	void updateOrthoProjectionMatrix(float width, float height, float length);
	void updateLightViewMatrix(glm::vec3 direction, glm::vec3 center);
};
#endif