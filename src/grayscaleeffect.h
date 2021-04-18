#ifndef GRAYSCALEEFFECT_H
#define GRAYSCALEEFFECT_H
#include "posteffect.h"

class GrayScaleEffect : public PostEffect {
public:
	GrayScaleEffect();
	~GrayScaleEffect();
	void update(Texture* mainTex, FrameBuffer* dest, Camera* cam, FrameBuffer* gBuffer);
	Material mat;
};
#endif