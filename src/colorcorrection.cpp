#include "colorcorrection.h"

ColorCorrection::ColorCorrection()
{
	mat.shader = ResourceManager::getInstance().load<ShaderProgram>("colorcorrection");
	exposure = 0.5f;
}

ColorCorrection::~ColorCorrection()
{
}

void ColorCorrection::update(Texture* mainTex, FrameBuffer* dest, Camera* cam, FrameBuffer* gBuffer)
{
	mat.setTempTexture("mainTex", mainTex);
	mat.setFloat("saturation", 1.1);
	mat.setFloat("contrast", 2.5);
	mat.setFloat("brightness", 0);
	mat.setFloat("exposure", exposure);
	blit(&mat, dest);
}
