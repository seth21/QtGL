#ifndef SHADOWBOX_H
#define SHADOWBOX_H
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include "frustum.h"
#include "camera.h"
#include <memory>
/**
 * Represents the 3D cuboidal area of the world in which objects will cast
 * shadows (basically represents the orthographic projection area for the shadow
 * render pass). It is updated each frame to optimise the area, making it as
 * small as possible (to allow for optimal shadow map resolution) while not
 * being too small to avoid objects not having shadows when they should.
 * Everything inside the cuboidal area represented by this object will be
 * rendered to the shadow map in the shadow render pass. Everything outside the
 * area won't be.
 *
 * @author Karl
 *
 */
class ShadowBox {
public:
	ShadowBox();
	void update(Camera* cam, glm::mat4 lightViewMatrix);
	glm::vec3 getCenter(glm::mat4 lightViewMatrix);
	float getWidth();
	float getHeight();
	float getLength();
	glm::vec3 getMin();
	glm::vec3 getMax();
	~ShadowBox();

private:
	//offset to not stop shadows as moving
	float OFFSET = 50;
	glm::vec4 UP = glm::vec4(0, 1, 0, 0);
	glm::vec4 FORWARD = glm::vec4(0, 0, -1, 0);
	float SHADOW_DISTANCE = 250;
	std::unique_ptr<Frustum> frustum;

	BoundingBox lightspaceBB;

};

#endif