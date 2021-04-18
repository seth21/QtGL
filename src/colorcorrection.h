#ifndef COLORCORRECTION_H
#define COLORCORRECTION_H
#include "posteffect.h"

class ColorCorrection : public PostEffect {
public:
	ColorCorrection();
	~ColorCorrection();
	void update(Texture* mainTex, FrameBuffer* dest, Camera* cam, FrameBuffer* gBuffer);
	Material mat;
	float exposure;
};
#endif