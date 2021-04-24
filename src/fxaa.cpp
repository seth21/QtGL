#include "fxaa.h"

FXAA::FXAA()
{
	mat.shader = ResourceManager::getInstance().load<ShaderProgram>("fxaa");
}

FXAA::~FXAA()
{
}

void FXAA::update(Texture* mainTex, FrameBuffer* dest, Camera* cam, FrameBuffer* gBuffer)
{
	mat.setTempTexture("mainTex", mainTex);
	blit(&mat, dest);
}