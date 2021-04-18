#include "grayscaleeffect.h"

GrayScaleEffect::GrayScaleEffect()
{
	mat.shader = ResourceManager::getInstance().load<ShaderProgram>("grayscale");
}

GrayScaleEffect::~GrayScaleEffect()
{
}

void GrayScaleEffect::update(Texture* mainTex, FrameBuffer* dest, Camera* cam, FrameBuffer* gBuffer)
{
	mat.setTempTexture("mainTex", mainTex);
	blit(&mat, dest);
}
