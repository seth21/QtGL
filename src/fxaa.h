#ifndef FXAA_H
#define FXAA_H

#include "posteffect.h"

class FXAA : public PostEffect {
public:
	FXAA();
	~FXAA();
	void update(Texture* mainTex, FrameBuffer* dest, Camera* cam, FrameBuffer* gBuffer);
	Material mat;
	float exposure;
};
#endif